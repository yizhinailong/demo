#pragma once

#include <QObject>

class PropertyWrapper : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString firstName READ firstName WRITE setFirstName NOTIFY firstNameChanged FINAL)
    Q_PROPERTY(QString lastName READ lastName WRITE setLastName NOTIFY lastNameChanged FINAL)

public:
    explicit PropertyWrapper(QObject* parent = nullptr);

    QString firstName() const;
    void setFirstName(const QString& newFirstName);

    QString lastName() const;
    void setLastName(const QString& newLastName);

signals:
    void firstNameChanged();

    void lastNameChanged();

private:
    QString m_firstName;
    QString m_lastName;
};
