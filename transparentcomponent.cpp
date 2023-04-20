#include "transparentcomponent.h"

#include <QPainter>
#include <QPaintEvent>
#include <QPainterPath>
#include <QLabel>
#include <QHBoxLayout>
#include <QPropertyAnimation>

const qreal ExternalContainerOpacity = 0;
const qreal TitleOpacity = 0.4;
const qreal BodyOpacity = 0.1;

const int bodyShowAnimationDuration = 600;
const int bodyHideAnimationDuration = 200;

TransparentComponent::TransparentComponent(QWidget *parent): QWidget(parent), opacity(ExternalContainerOpacity)
{
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
}

void TransparentComponent::setOpacity(qreal newOpacity)
{
    opacity = newOpacity;
    update();
}

void TransparentComponent::paintEvent(QPaintEvent *event)
{
    QPainter painters(this);
    QPainterPath path;
    path.addRoundedRect(event->rect(), 6, 6);
    painters.setClipPath(path);
    painters.setOpacity(opacity);
    painters.fillRect(event->rect(), Qt::black);
}

TransparentComponentTitle::TransparentComponentTitle(QWidget *parent):TransparentComponent(parent)
{
    setOpacity(TitleOpacity);
    setFixedHeight(50);
    auto font = this->font();
    font.setPixelSize(18);
    setFont(font);
}

void TransparentComponentTitle::setText(const QString &text)
{
    title = text;
    update();
}

void TransparentComponentTitle::setBody(TransparentComponentBody *body)
{
    assert(body);
    this->body = body;
}

qreal TransparentComponentTitle::getAngle() const
{
    return angle;
}

void TransparentComponentTitle::setAngle(qreal newAngle)
{
    if(qFuzzyCompare(angle, newAngle))
        return;
    angle = newAngle;
}

void TransparentComponentTitle::mousePressEvent(QMouseEvent *event)
{
    if(bodyState != state::AnimationRuning)
    {
        if(bodyState == state::BodyIsHide)
        {
            body->show();
        }

        auto animationAngle = new QPropertyAnimation(this, "angle");
        animationAngle->setEasingCurve(QEasingCurve::OutBounce);
        animationAngle->setStartValue(angle);
        animationAngle->setEndValue(bodyState == state::BodyIsHide ? 180 : 0);
        animationAngle->setDuration(bodyState == state::BodyIsHide ? bodyShowAnimationDuration : bodyHideAnimationDuration);

        connect(animationAngle, &QPropertyAnimation::valueChanged, [this]{update();});
        auto animationAliasOfHeight = new QPropertyAnimation(body, "aliasOfHeight");
        if(bodyState == state::BodyIsHide)
        {
            animationAliasOfHeight->setStartValue(0);
            animationAliasOfHeight->setEndValue(200);
            animationAliasOfHeight->setEasingCurve(QEasingCurve::OutBounce);
            animationAliasOfHeight->setDuration(bodyShowAnimationDuration);
            connect(animationAliasOfHeight,&QPropertyAnimation::finished,[this]
            {
                bodyState = state::BodyIsShow;
            });
        }
        else
        {
            animationAliasOfHeight->setStartValue(body->height());
            animationAliasOfHeight->setEndValue(0);
            animationAliasOfHeight->setDuration(bodyHideAnimationDuration);
            connect(animationAliasOfHeight,&QPropertyAnimation::finished,[this]
            {
                bodyState = state::BodyIsHide;
                body->hide();
            });
        }
        animationAliasOfHeight->start(QAbstractAnimation::DeleteWhenStopped);
        animationAngle->start(QAbstractAnimation::DeleteWhenStopped);
        bodyState = state::AnimationRuning;
    }
    QWidget::mousePressEvent(event);

}

void TransparentComponentTitle::paintEvent(QPaintEvent *event)
{
    TransparentComponent::paintEvent(event);
    const auto drawRect = event->rect();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::white);
    painter.drawText(drawRect.adjusted(15, 0, 0, 0), Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, title);

    painter.setPen(QPen(bodyState == state::BodyIsShow ? Qt::white : QColor("#C2A9A6"), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    QRect iconRect(drawRect.width() - 25, 20, 10, 10);
    painter.translate(iconRect.center());
    painter.rotate(angle);
    iconRect = QRect(-5, -5, 10, 10);
    painter.drawLine(iconRect.topLeft(), QPoint(iconRect.topLeft().x() + 5, iconRect.bottom()));
    painter.drawLine(QPoint(iconRect.topLeft().x() + 5,iconRect.bottom()),iconRect.topRight());


}

TransparentComponentBody::TransparentComponentBody(QWidget *parent) : TransparentComponent(parent)
{
    setOpacity(BodyOpacity);
    auto label = new QLabel(this);
    label->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    label->setWordWrap(true);
    label->setStyleSheet("color:white; font-size:16px;");
    auto hb = new QHBoxLayout(this);
    hb->setContentsMargins(20, 20, 20, 20);
    hb->addWidget(label);
    this->hide();
}

void TransparentComponentBody::setText(const QString &text)
{
    findChild<QLabel*>()->setText(text);
}

void TransparentComponentBody::setAliasOfHeight(int height)
{
    setFixedHeight(height);
}

int TransparentComponentBody::getAliasOfHeight()
{
    return this->height();
}

AccordionZoomWidget::AccordionZoomWidget(QWidget *parent):TransparentComponent(parent)
{
    auto vb = new QVBoxLayout(this);
    vb->setContentsMargins(0, 0, 0, 3);
    vb->setSpacing(0);
    title = new TransparentComponentTitle(this);
    body = new TransparentComponentBody(this);
    title->setBody(body);
    vb->addWidget(title);
    vb->addWidget(body);
}

void AccordionZoomWidget::setText(const QString &text)
{
    body->setText(text);
}

void AccordionZoomWidget::setTitle(const QString &text)
{
    title->setText(text);
}
