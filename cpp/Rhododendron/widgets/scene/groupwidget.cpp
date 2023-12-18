#include "groupwidget.h"
#include "ui_groupwidget.h"

GroupWidget::GroupWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupWidget)
{
    ui->setupUi(this);
    QList<QString> header;
    header.append("Group");
    header.append("Name");
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

GroupWidget::~GroupWidget()
{
    delete ui;
}


void GroupWidget::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}

void GroupWidget::on_lineEdit_returnPressed()
{
    on_pushButton_clicked();
}


void GroupWidget::on_pushButton_clicked()
{
    auto name = ui->lineEdit->text();
    std::set<Lamp*> lamps;
    for(auto &lamp : selection)
    {
        for(auto &group : groups)
        {
            if(group.second.count(lamp))
                group.second.erase(lamp);

            if(group.second.size() == 0)
                groups.erase(group.first);
        }
        lamps.insert(lamp);
    }
    if(groups.count(name))
    {
        for(auto &lamp : lamps)
        {
            groups[name].insert(lamp);
        }
    }
    else
    {
        groups.insert(std::make_pair(name, lamps));
    }


    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    for(auto &group : groups)
    {
        for(auto &lamp : group.second)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount());
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 0, new QTableWidgetItem(group.first));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 1, new QTableWidgetItem(lamp->name));
        }
    }
    selection.clear();
}


void GroupWidget::updateSelection(QGraphicsItemGroup &selection)
{
    this->selection.clear();
    for(auto &item : selection.childItems())
    {
        auto proxy = static_cast<QGraphicsProxyWidget*>(item);
        auto lampWidget = static_cast<LampWidget*>(proxy->widget());

        this->selection.insert(lampWidget->getLamp());
    }
    ui->lineEdit->setText("group " + QString::number(groups.size() + 1));
}
