#pragma once 

#include <QAbstractListModel>
#include <QStringList>

//![0]
class Project
{
    enum class STATUS {
          ACTIVE
        , INACTIVE
    };

public:
    Project();
    Project(int64_t id, const QString& logoUrl, const QString& name, const QString& status, int64_t timeSpentWeek, int64_t timeSpentMonth, int64_t timeSpentTotal);
    
    QString id() const;
    QString name() const;   
    QString logoUrl() const;
    QString status() const;
    QString timeSpentWeek() const;
    QString timeSpentMonth() const;
    QString timeSpentTotal() const;

private:
    struct {
        int64_t id;
        QString logoUrl;
        QString name;
        STATUS status;
        int64_t timeSpentWeek;
        int64_t timeSpentMonth;
        int64_t timeSpentTotal;
    } data;
};

class ProjectModel : public QAbstractListModel
{
    Q_OBJECT

public slots:
    void insertProjects(const QList<Project>& projects);
    void insertProjectsV(const QVariantList& projects);

public:
    enum ProjectRoles {           
          NameRole = Qt::UserRole + 1
        , LogoUrlRole
        , StatusRole
        , TimeSpentWeekRole
        , TimeSpentMonthRole
        , TimeSpentTotalRole
        , IDRole
    };

    ProjectModel(QObject* parent = 0);
    
    int rowCount(const QModelIndex& parent = QModelIndex()) const;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    struct {
        QList<Project> projects;
    } modelData;
};

Q_DECLARE_METATYPE(Project)