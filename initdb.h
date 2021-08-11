#ifndef INITDB_H
#define INITDB_H
#include <QDate>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QVariant>

void addDocument(QSqlQuery &q, const QVariant &n_doc, const QDate &date_doc, const QVariant &id_enterp)
{
    q.bindValue(":n_doc", n_doc);
    q.bindValue(":date_doc", date_doc);
    q.bindValue(":id_enterp", id_enterp);
    q.exec();
}

const QVariant addEnterp(QSqlQuery &q, const QString &name, const QString inn, const QString kpp){
    q.bindValue(":name", name);
    q.bindValue(":inn", inn);
    q.bindValue(":kpp", kpp);
    if (!q.exec())
        return QVariant();
    return q.lastInsertId();
}

const auto ENTERP_SQL = "create table enterp(id integer primary key, name varchar, inn varchar, kpp varchar)";

const auto DOCUMENTS_SQL =  "create table document(id integer primary key, n_doc varchar, date_doc date, id_enterp integer)";

const auto VUDOCUMENTS_SQL =  "create view vudocuments as "
                              "select d.id, d.n_doc, d.date_doc, d.id_enterp, e.name, e.inn, e.kpp "
                              " from document d left join enterp e "
                              "on d.id_enterp = e.id ";

const auto INSERT_DOC_SQL = "insert into document(n_doc, date_doc, id_enterp) "
                            "values(:n_doc, :date_doc, :id_enterp)";

const auto INSERT_ENTERP_SQL = "insert into enterp(name, inn, kpp) "
                               "values(:name, :inn, :kpp)";

QSqlError initDb()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");

    if (!db.open())
        return db.lastError();

    QSqlQuery q;
    if (!q.exec(ENTERP_SQL))
        return q.lastError();
    if (!q.exec(DOCUMENTS_SQL))
        return q.lastError();
    if (!q.exec(VUDOCUMENTS_SQL))
        return q.lastError();


    if (!q.prepare(INSERT_ENTERP_SQL))
        return q.lastError();
    auto enterp_id1 = addEnterp(q, "ОАО Рога и Копыта", "2900100101", "29001001");
    auto enterp_id2 = addEnterp(q, "ОАО Копыта", "7700100101", "77001001");

    if (!q.prepare(INSERT_DOC_SQL))
        return q.lastError();
    addDocument(q, "1/1", QDate::currentDate(), enterp_id1);
    addDocument(q, "1/2", QDate::currentDate(), enterp_id1);
    addDocument(q, "1/2", QDate::currentDate(), enterp_id2);
    addDocument(q, "2/2", QDate::currentDate(), enterp_id2);

    return QSqlError();
}
#endif // INITDB_H
