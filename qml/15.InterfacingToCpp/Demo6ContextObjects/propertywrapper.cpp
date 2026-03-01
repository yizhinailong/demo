#include "propertywrapper.hpp"

PropertyWrapper::PropertyWrapper(QObject* parent)
    : QObject{ parent },
      m_firstName("FirstName"),
      m_lastName("LastName") {
}

QString PropertyWrapper::firstName() const {
    return m_firstName;
}

void PropertyWrapper::setFirstName(const QString& newFirstName) {
    if (m_firstName == newFirstName) {
        return;
    }
    m_firstName = newFirstName;
    emit firstNameChanged();
}

QString PropertyWrapper::lastName() const {
    return m_lastName;
}

void PropertyWrapper::setLastName(const QString& newLastName) {
    if (m_lastName == newLastName) {
        return;
    }
    m_lastName = newLastName;
    emit lastNameChanged();
}
