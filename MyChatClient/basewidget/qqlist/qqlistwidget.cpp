#include "qqlistwidget.h"
#include "unit.h"

#include <QDebug>


QQListWidget::QQListWidget(QWidget *parent) :
    QListWidget(parent)
{
    m_nRightClickCell = NULL;
}

// 获取右键点击的单元格
QQCell *QQListWidget::GetRightClickedCell()
{
    return m_nRightClickCell;
}

// 设置分组弹出菜单
void QQListWidget::setGroupPopMenu(QMenu *menu)
{
    mGroupMenu = menu;
}

// 设置子项弹出菜单
void QQListWidget::setChildPopMenu(QMenu *menu)
{
    mChildMenu = menu;
}

// 插入QQCell对象
void QQListWidget::insertQQCell(QQCell *cell)
{
    // 如果cell是分组类型
    if (cell->type == QQCellType_Group ||
        cell->type == QQCellType_GroupEx)
    {
        cells.append(cell);  // 直接添加到cells中
    }
    else
    {
        // 子项类型
        foreach (QQCell *group, cells)
        {
            if (!group->groupName.compare(cell->groupName))//找到选中的分组加入
            {
                group->childs.append(cell);  // 添加为分组的子项
                break;
            }
        }
    }

    upload();  // 调用更新函数
}

// 移除QQCell对象
void QQListWidget::removeQQCell(QQCell *cell)
{
    // 如果cell是分组类型
    if (cell->type == QQCellType_Group ||
        cell->type == QQCellType_GroupEx)
    {
        cells.removeOne(cell);  // 从cells中移除
    }
    else
    {
        // 子项类型
        for (QQCell *group : cells)
        {
            if (!group->groupName.compare(cell->groupName))
            {
                group->childs.removeOne(cell);  // 从分组的子项中移除
                break;
            }
        }
    }

    upload();  // 调用更新函数
}

/**
 * @brief QQListWidget::upload
 * 更新列表显示
 */
void QQListWidget::upload()
{
    this->clear();  // 清空当前列表
    // 刷新列表
    for (QQCell *cell : cells)
    {
        if (cell->type == QQCellType_Group || cell->type == QQCellType_GroupEx)
        {
            QQListViewGroup *group = new QQListViewGroup();  // 创建新的分组视图
            group->setGeometry(0, 0, 300, 30);  // 设置分组尺寸
            if (cell->type == QQCellType_Group)
            {
                // 获取在线好友数量
                int nOnlineCnt = 0;
                for (QQCell *child : cell->childs)
                {
                    if (OnLine == child->status) {
                        nOnlineCnt++;  // 在线好友计数
                    }
                }

                // 设置标题和子标题
                group->ui->groupTitleLabel->setText(cell->groupName);
                group->ui->groupSubTitleLabel->setText(QString("[%1/%2]").arg(nOnlineCnt).arg(cell->childs.size()));
            }
            else
            {
                group->ui->groupTitleLabel->setText(QString("%1[%2/%2]").arg(cell->groupName).arg(cell->childs.size()));
                group->ui->groupSubTitleLabel->setText(QString(""));
            }
            group->setQQCell(cell);  // 设置group的QQCell
            group->setPopMenu(mGroupMenu);  // 设置分组弹出菜单
            // 连接信号和槽
            connect(group, SIGNAL(onGroupOpenStatusDidChanged(QQListViewGroup*)),
                    this, SLOT(onGroupOpenDidChanged(QQListViewGroup *)));
            connect(group, SIGNAL(onpopmenuwillshow(QQPopMenuWidget*,QMenu*)),
                    this, SLOT(on_popmenuWillShow(QQPopMenuWidget*,QMenu*)));

            QListWidgetItem *item = new QListWidgetItem("");
            this->addItem(item);  // 将分组添加到列表
            this->setItemWidget(item, group);  // 设置项目的widget
            item->setSizeHint(group->geometry().size());  // 设置项目的大小

            if (cell->isOpen)  // 如果分组是打开状态
            {
                // 先显示在线好友
                for (QQCell *ccell : cell->childs)
                {
                    if (OnLine == ccell->status)
                        AddChildItem(ccell);
                }

                // 后显示离线好友
                for (QQCell *ccell : cell->childs)
                {
                    if (OffLine == ccell->status)
                        AddChildItem(ccell);
                }
            }
        }
    }
}

// 处理分组开关状态变化
void QQListWidget::onGroupOpenDidChanged(QQListViewGroup */*group*/)
{
    upload();  // 重新更新列表
}

// 处理子项选择事件
void QQListWidget::onChildDidSelected(QQListViewChild *child)
{
    // qDebug() << "clicked" << child->cell->name;
    Q_UNUSED(child);
}

// 设置背景颜色
void QQListWidget::setBackGroundColor(QWidget *widget, int index)
{
    // 根据index设置背景颜色，奇数为灰色，偶数为白色
    widget->setStyleSheet(index % 2 ? "QWidget{background:rgb(100, 100, 100, 100)}" :
                                      "QWidget{background:rgb(255, 255, 255)}");
}

/**
 * @brief QQListWidget::AddChildItem
 * 添加子项至列表
 * @param cell
 */
void QQListWidget::AddChildItem(QQCell *cell)
{
    QQListViewChild *child = new QQListViewChild;  // 创建新的子项视图
    child->setGeometry(0, 0, 300, 60);  // 设置尺寸
    child->cell = cell;  // 设置cell对象
    child->setPopMenu(mChildMenu);  // 设置子项弹出菜单
    // 连接信号和槽
    connect(child, SIGNAL(onChildDidSelected(QQListViewChild*)), this,
            SLOT(onChildDidSelected(QQListViewChild*)));
    connect(child, SIGNAL(onChildDidDoubleClicked(QQCell*)),
            this, SIGNAL(onChildDidDoubleClicked(QQCell*)));
    connect(child, SIGNAL(signalRightClicked(QQCell*)),
            this, SLOT(SltCellRightCicked(QQCell*)));

    QListWidgetItem *item = new QListWidgetItem("");
    this->addItem(item);  // 添加子项到列表
    this->setItemWidget(item, child);  // 设置项目的widget
    item->setSizeHint(child->geometry().size());  // 设置项目的大小
}

// 根据分组名称获取分组对象
QQCell *QQListWidget::getGroupForName(QString *groupName)
{
    for (QQCell *group : cells)
    {
        if (!group->groupName.compare(groupName))  // 比较名称
        {
            return group;  // 返回找到的分组
        }
    }

    return NULL;  // 如果未找到，则返回NULL
}

// 处理弹出菜单显示事件
void QQListWidget::on_popmenuWillShow(QQPopMenuWidget *widget, QMenu *menu)
{
    emit on_popmenu_will_show(this, widget, menu);  // 发出信号
}

// 处理单元格右键点击事件
void QQListWidget::SltCellRightCicked(QQCell *cell)
{
    m_nRightClickCell = cell;  // 记录右键点击的单元格
}

// 获取cells列表
QList<QQCell *> QQListWidget::getCells() const
{
    return cells;  // 返回cells列表
}
