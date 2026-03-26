#pragma once
#include <QObject>
#include <QString>
#include <QTimer>
#include <QDateTime>

class SessionManager : public QObject {
    Q_OBJECT
public:
    explicit SessionManager(const QString& sessionFile, QObject* parent = nullptr);

    bool    tryRestoreSession();
    void    startSession(const QString& username);
    void    endSession();
    bool    isActive() const;
    QString currentUser() const;
    int     remainingSeconds() const;

    static constexpr int SESSION_DURATION_SECS = 300; // 5 minutos

signals:
    void sessionExpired();
    void tickSecond(int remainingSecs);

private slots:
    void onTick();

private:
    QString   m_sessionFile;
    QString   m_currentUser;
    QDateTime m_sessionStart;
    QTimer*   m_timer;
    bool      m_active = false;

    bool      saveSession();
    void      clearSession();
};
