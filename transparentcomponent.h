#ifndef TRANSPARENTCOMPONENT_H
#define TRANSPARENTCOMPONENT_H

#include <QWidget>

class TransparentComponent : public QWidget
{
    Q_OBJECT
public:
    TransparentComponent(QWidget *parent = nullptr);
    void setOpacity(qreal newOpacity);

protected:
    void paintEvent(QPaintEvent *event) override;
private:
    qreal opacity;
};

class TransparentComponentTitle : public TransparentComponent
{
    Q_OBJECT
    Q_PROPERTY(qreal angle READ getAngle WRITE setAngle MEMBER angle NOTIFY angleChanged)

public:
    TransparentComponentTitle(QWidget *parent = nullptr);
    void setText(const QString &text);
    void setBody(class TransparentComponentBody *body);

    qreal getAngle() const;
    void  setAngle(qreal newAngle);

signals:
    void angleChanged();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QString title;
    TransparentComponentBody *body;
    enum class state
    {
        BodyIsHide,
        AnimationRuning,
        BodyIsShow
    };
    state bodyState{state::BodyIsHide};
    qreal angle{0};
};

class TransparentComponentBody : public TransparentComponent
{
    Q_OBJECT
    Q_PROPERTY(int aliasOfHeight READ getAliasOfHeight WRITE setAliasOfHeight)
public:
    TransparentComponentBody(QWidget *parent = nullptr);
    void setText(const QString &text);
    void setAliasOfHeight(int height);
    int getAliasOfHeight();
};

class AccordionZoomWidget : public TransparentComponent
{
    Q_OBJECT
public:
    AccordionZoomWidget(QWidget *parent = nullptr);
    void setText(const QString &text);
    void setTitle(const QString &text);

private:
    TransparentComponentTitle *title;
    TransparentComponentBody *body;
};

#endif // TRANSPARENTCOMPONENT_H
