#include "projectsmodel.h"

#include <QDateTime>

Project::Project()
{
}

Project::Project(int64_t id, const QString& logoUrl, const QString& name, const QString& status, int64_t timeSpentWeek, int64_t timeSpentMonth, int64_t timeSpentTotal)
{
    data.id = id;
    data.logoUrl = logoUrl;
    data.name = name;
    data.status = status.toInt() == 1 ? STATUS::ACTIVE : STATUS::INACTIVE;
    data.timeSpentWeek = timeSpentWeek;
    data.timeSpentMonth = timeSpentMonth;
    data.timeSpentTotal = timeSpentTotal;
}

QString Project::id() const
{
    return QString::number(data.id);
}

QString Project::name() const
{
    return data.name;
}

QString Project::logoUrl() const
{
    return data.logoUrl;
}

QString Project::status() const
{
    return data.status == STATUS::ACTIVE ?
        QStringLiteral("Active") : data.status == STATUS::INACTIVE ?
        QStringLiteral("Inactive") : QStringLiteral("Unknown");
}

QString Project::timeSpentWeek() const
{
    return QDateTime::fromTime_t(data.timeSpentWeek).toUTC().toString("hh:mm:ss");    
}

QString Project::timeSpentMonth() const
{
    return QDateTime::fromTime_t(data.timeSpentMonth).toUTC().toString("hh:mm:ss");
}

QString Project::timeSpentTotal() const
{
    return QDateTime::fromTime_t(data.timeSpentTotal).toUTC().toString("hh:mm:ss");
}

ProjectModel::ProjectModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

#if 0
void ProjectModel::addAnimal(const Animal& animal)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_animals << animal;
    endInsertRows();
}
#endif

int ProjectModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);

    return modelData.projects.count();
}

QVariant ProjectModel::data(const QModelIndex& index, int role) const {
    if (index.row() < 0 || index.row() >= modelData.projects.count()) {
        return QVariant();
    }

    const auto& project = modelData.projects[index.row()];
        
    if (role == NameRole) {
        return project.name();
    } else if (role == LogoUrlRole) {
        return project.logoUrl();
    } else if (role == StatusRole) {
        return project.status();
    } else if (role == TimeSpentWeekRole) {
        return project.timeSpentWeek();
    } else if (role == TimeSpentMonthRole) {
        return project.timeSpentMonth();
    } else if (role == TimeSpentTotalRole) {
        return project.timeSpentTotal();
    } else if (role == IDRole) {
        return project.id();
    }
    
    return QVariant();
}

QHash<int, QByteArray> ProjectModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[NameRole] = "name";    
    roles[LogoUrlRole] = "logourl";
    roles[StatusRole] = "status";
    roles[TimeSpentWeekRole] = "timespentweek";
    roles[TimeSpentMonthRole] = "timespentmonth";
    roles[TimeSpentTotalRole] = "timespenttotal";
    roles[IDRole] = "id";

    return roles;
}

void ProjectModel::insertProjects(const QList<Project>& projects)
{
    beginResetModel();
        modelData.projects.clear();
        modelData.projects = projects;
    endResetModel();
}

void ProjectModel::insertProjectsV(const QVariantList& projects)
{
    beginResetModel();
        modelData.projects.clear();

        for (const auto& project : projects) {
            modelData.projects << project.value<Project>();
        }
    endResetModel();
}
