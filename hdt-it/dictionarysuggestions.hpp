#ifndef DICTIONARYSUGGESTIONS_HPP
#define DICTIONARYSUGGESTIONS_HPP

#include <QtGui>
#include <QObject>

#include <hdtmanager.hpp>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QTimer;
class QTreeWidget;
QT_END_NAMESPACE

class DictionarySuggestions : public QObject
{
    Q_OBJECT
public:
    explicit DictionarySuggestions(QLineEdit *parent = 0);
    ~DictionarySuggestions();
    bool eventFilter(QObject *obj, QEvent *ev);
    void showCompletion(const QStringList &choices);
    void setManager(HDTManager *manager);
    void setRole(hdt::TripleComponentRole role);

public slots:
    void doneCompletion();
    void preventSuggest();
    void autoSuggest();

private:
    QLineEdit *editor;
    QTreeWidget *popup;
    QTimer *timer;
    HDTManager *manager;
    hdt::TripleComponentRole role;
};

#endif // DICTIONARYSUGGESTIONS_HPP
