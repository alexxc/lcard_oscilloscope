#include "LogPanel.h"
#include "Log.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTableView>
#include <QMenu>
#include <QAction>
#include <QEvent>
#include <QHeaderView>
#include <QScrollBar>
#include <QSettings>

#include "actions/TableCopyAction.h"

namespace LQMeas {
    LogPanel::LogPanel(QWidget *parent) :
        QDockWidget(parent)  {

        this->setObjectName("LQMeasLogPanel");

        m_model = new LogModel(this);

        m_logbox = new QTableView();
        m_logbox->horizontalHeader()->setVisible(false);
        m_logbox->verticalHeader()->setVisible(false);
        m_logbox->horizontalHeader()->setStretchLastSection(true);
        m_logbox->setEditTriggers(QAbstractItemView::NoEditTriggers);
        m_logbox->setSelectionBehavior(QAbstractItemView::SelectRows);
        m_logbox->setMinimumWidth(320);

        m_logbox->setStyleSheet("QTableView::item { padding: 3px;}");

        m_logbox->setModel(m_model);


        m_logbox->setColumnWidth(0, 175);
        m_logbox->setColumnWidth(1, 135);

        connect(m_model, SIGNAL(rowsAboutToBeInserted(const QModelIndex &, int , int )),
                SLOT(checkScroll()));

        connect(m_model, SIGNAL(rowsInserted(const QModelIndex &, int , int )),
                SLOT(onInserted(QModelIndex,int,int)));


        checkScroll();

        m_actClear = new QAction(m_logbox);
        m_actClear->setIcon(QIcon(":/icons/clear.png"));
        connect(m_actClear, SIGNAL(triggered()), SLOT(clear()));
        m_actClear->setShortcut(tr("Ctrl+D"));
        m_logbox->addAction(m_actClear);

        m_logbox->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(m_logbox, SIGNAL(customContextMenuRequested(QPoint)), SLOT(showContextMenu(QPoint)));

        m_contextMenu = new QMenu(this);


        m_actCopy = new TableCopyAction(m_logbox);
        m_actCopy->setIcon(QIcon(":/icons/copy.png"));
        m_logbox->addAction(m_actCopy);
        m_contextMenu->addAction(m_actCopy);

        m_actSelectAll= new QAction(this);
        m_actSelectAll->setIcon(QIcon(":/icons/select_all.png"));
        m_actSelectAll->setShortcut(QKeySequence::SelectAll);
        m_logbox->addAction(m_actSelectAll);
        m_contextMenu->addAction(m_actSelectAll);
        connect(m_actSelectAll, SIGNAL(triggered()), m_logbox, SLOT(selectAll()));


        m_contextMenu->addAction(m_actClear);


        m_levelMenu = new QMenu();
        m_contextMenu->addMenu(m_levelMenu);

        QActionGroup *lvlActions = new QActionGroup(m_levelMenu);
        QList<LogLevel> lvls = Log::instance()->logLevels();

        foreach (LogLevel lvl, lvls) {
            QAction* act = new QAction(lvl.name(), lvlActions);
            act->setCheckable(true);
            if (lvl.number() == m_model->curLogLevel().number())
                act->setChecked(true);
            m_logLvlActMap[act] = lvl.number();
            lvlActions->addAction(act);
        }
        connect(lvlActions, SIGNAL(triggered(QAction*)), SLOT(lvlActTriggered(QAction*)));

        m_levelMenu->addActions(lvlActions->actions());



        m_actResize = new QAction(this);
        connect(m_actResize, SIGNAL(triggered()), this, SLOT(resizeRows()));
        m_contextMenu->addAction(m_actResize);


        retranslateUi();


        setWidget(m_logbox);



        connect(this, SIGNAL(visibilityChanged(bool)), this,
                SLOT(resizeRows()));
    }

    LogPanel::~LogPanel() {

    }

    void LogPanel::clear() {
        m_model->clear();
    }

    void LogPanel::resizeRows() {
        m_logbox->resizeRowsToContents();
        m_logbox->repaint();
    }

    void LogPanel::changeEvent(QEvent *event) {
        switch (event->type()) {
            case QEvent::LanguageChange:
                retranslateUi();
                break;
        }

        QDockWidget::changeEvent(event);
    }

    void LogPanel::showContextMenu(QPoint point) {
        m_contextMenu->popup(m_logbox->mapToGlobal(point));
    }

    void LogPanel::lvlActTriggered(QAction *act) {
        m_model->setLogLevel(Log::instance()->level(m_logLvlActMap[act]));
    }

    void LogPanel::retranslateUi() {
        setWindowTitle(tr("Log"));
        m_levelMenu->setTitle(tr("Show log level"));
        m_actClear->setText(tr("Clear"));
        m_actSelectAll->setText(tr("Select All"));
        m_actResize->setText(tr("Recalculate rows size"));
        m_actCopy->retranslate();
        foreach (QAction *act, m_logLvlActMap.keys()) {
            act->setText(Log::instance()->level(m_logLvlActMap[act]).name());
        }
    }

    void LogPanel::checkScroll() {
        QScrollBar* bar = m_logbox->verticalScrollBar();
        bool at_end = bar->value() == bar->maximum();

        if (at_end) {
           connect(m_model, SIGNAL(rowsInserted(const QModelIndex &, int , int )),
                   m_logbox, SLOT(scrollToBottom()));
        } else {
           disconnect(m_model, SIGNAL(rowsInserted(const QModelIndex &, int , int )),
                      m_logbox, SLOT(scrollToBottom()));
        }


    }

    void LogPanel::onInserted(const QModelIndex &, int first, int last) {
        for (int i=first; i <= last; i++) {
            m_logbox->resizeRowToContents(i);
        }
    }
}
