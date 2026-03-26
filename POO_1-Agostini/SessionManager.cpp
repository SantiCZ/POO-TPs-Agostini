#include "SessionManager.h"
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>

SessionManager::SessionManager(const QString& sessionFile, QObject* parent)
    : QObject(parent), m_sessionFile(sessionFile) {
    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    connect(m_timer, &QTimer::timeout, this, &SessionManager::onTick);
}

bool SessionManager::tryRestoreSession() {
    QFile file(m_sessionFile);
    if (!file.exists() || !file.open(QIODevice::ReadOnly)) return false;
    QJsonObject obj = QJsonDocument::fromJson(file.readAll()).object();
    file.close();

    QString   user = obj["username"].toString();
    QDateTime ts   = QDateTime::fromString(obj["timestamp"].toString(), Qt::ISODate);
    if (user.isEmpty() || !ts.isValid()) return false;

    int elapsed = static_cast<int>(ts.secsTo(QDateTime::currentDateTime()));
    if (elapsed >= SESSION_DURATION_SECS) { clearSession(); return false; }

    m_currentUser  = user;
    m_sessionStart = ts;
    m_active       = true;
    m_timer->start();
    return true;
}

void SessionManager::startSession(const QString& username) {
    m_currentUser  = username;
    m_sessionStart = QDateTime::currentDateTime();
    m_active       = true;
    saveSession();
    m_timer->start();
}

void SessionManager::endSession() {
    m_active = false;
    m_currentUser.clear();
    m_timer->stop();
    clearSession();
}

bool  SessionManager::isActive()      const { return m_active; }
QString SessionManager::currentUser() const { return m_currentUser; }

int SessionManager::remainingSeconds() const {
    if (!m_active) return 0;
    int elapsed = static_cast<int>(m_sessionStart.secsTo(QDateTime::currentDateTime()));
    return qMax(0, SESSION_DURATION_SECS - elapsed);
}

void SessionManager::onTick() {
    int rem = remainingSeconds();
    emit tickSecond(rem);
    if (rem <= 0) {
        m_active = false;
        m_timer->stop();
        clearSession();
        emit sessionExpired();
    }
}

bool SessionManager::saveSession() {
    QDir().mkpath(QFileInfo(m_sessionFile).absolutePath());
    QFile file(m_sessionFile);
    if (!file.open(QIODevice::WriteOnly)) return false;
    QJsonObject obj;
    obj["username"]  = m_currentUser;
    obj["timestamp"] = m_sessionStart.toString(Qt::ISODate);
    file.write(QJsonDocument(obj).toJson());
    file.close();
    return true;
}

void SessionManager::clearSession() { QFile::remove(m_sessionFile); }
