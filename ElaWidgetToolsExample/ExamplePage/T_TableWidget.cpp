#include "T_TableWidget.h"

#include <QHBoxLayout>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QVBoxLayout>

#include "ElaScrollBar.h"
#include "ElaTableWidget.h"
#include "ElaText.h"

T_TableWidget::T_TableWidget(QWidget* parent)
    : T_BasePage(parent)
{
    // 预览窗口标题
    setWindowTitle("ElaTableWidget");

    // 顶部元素
    createCustomWidget("表格部件被放置于此，可在此界面体验其效果并按需添加进项目中");

    // ElaTableWidget
    ElaText* tableText = new ElaText("ElaTableWidget", this);
    tableText->setTextPixelSize(18);

    _tableWidget = new ElaTableWidget(this);
    _tableWidget->setFixedHeight(450);

    // 配置表格
    _tableWidget->setColumnCount(5);
    _tableWidget->setRowCount(9);

    // 设置表头
    QStringList headers;
    headers << "预览"
            << "歌名"
            << "歌手"
            << "专辑"
            << "时长";
    _tableWidget->setHorizontalHeaderLabels(headers);

    // 配置表头外观
    QFont tableHeaderFont = _tableWidget->horizontalHeader()->font();
    tableHeaderFont.setPixelSize(16);
    _tableWidget->horizontalHeader()->setFont(tableHeaderFont);
    _tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    _tableWidget->horizontalHeader()->setMinimumSectionSize(60);

    // 隐藏垂直表头
    _tableWidget->verticalHeader()->setHidden(true);
    _tableWidget->verticalHeader()->setMinimumSectionSize(46);

    // 配置表格行为
    _tableWidget->setAlternatingRowColors(true);
    _tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    _tableWidget->setIconSize(QSize(38, 38));

    // 添加示例数据
    QStringList songData[9] = {
        {"夜航星(Night Voyager)", "不才/三体宇宙", "我的三体之章北海传", "05:03"},
        {"玫瑰少年", "五月天", "玫瑰少年", "03:55"},
        {"Collapsing World(Original Mix)", "Lightscape", "Collapsing World", "03:10"},
        {"RAIN MAN (雨人)", "AKIHIDE (佐藤彰秀)", "RAIN STORY", "05:37"},
        {"黑暗森林", "雲翼星辰", "黑暗森林", "05:47"},
        {"轻(我的三体第四季主题曲)", "刘雪茗", "我的三体第四季", "01:59"},
        {"STYX HELIX", "MYTH & ROID", "STYX HELIX", "04:51"},
        {"LAST STARDUST", "Aimer", "DAWN", "05:18"},
        {"Running In The Dark", "MONKEY MAJIK/塞壬唱片", "Running In The Dark", "03:40"}};

    QString iconPaths[9] = {
        ":/Resource/Image/Model/NaightNavigationStar.jpg",
        ":/Resource/Image/Model/MaVieEnRose.jpg",
        ":/Resource/Image/Model/CollapsingWorld.jpg",
        ":/Resource/Image/Model/RainMan.jpg",
        ":/Resource/Image/Model/DarkForest.jpg",
        ":/Resource/Image/Model/Light.jpg",
        ":/Resource/Image/Model/STYXHELIX.jpg",
        ":/Resource/Image/Model/LASTSTARDUST.jpg",
        ":/Resource/Image/Model/RunningInTheDark.jpg"};

    for (int row = 0; row < 9; ++row)
    {
        // 预览列 - 图标
        QTableWidgetItem* previewItem = new QTableWidgetItem();
        QIcon icon(QPixmap(iconPaths[row]).scaled(38, 38, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        previewItem->setIcon(icon);
        previewItem->setTextAlignment(Qt::AlignCenter);
        _tableWidget->setItem(row, 0, previewItem);

        // 歌曲数据列
        for (int col = 0; col < 4; ++col)
        {
            QTableWidgetItem* item = new QTableWidgetItem(songData[row][col]);
            if (col == 3)
            {
                // 时长列居中对齐
                item->setTextAlignment(Qt::AlignCenter);
            }
            _tableWidget->setItem(row, col + 1, item);
        }

        // 为前三行设置不同的颜色
        if (row == 0)
        {
            // 第一行 - 红色背景
            for (int col = 0; col < 5; ++col)
            {
                _tableWidget->item(row, col)->setBackground(QColor(255, 150, 150));
            }
        }
        else if (row == 1)
        {
            // 第二行 - 绿色背景
            for (int col = 0; col < 5; ++col)
            {
                _tableWidget->item(row, col)->setBackground(QColor(150, 255, 150));
            }
        }
        else if (row == 2)
        {
            // 第三行 - 蓝色文字（不改变背景）
            for (int col = 0; col < 5; ++col)
            {
                _tableWidget->item(row, col)->setForeground(QColor(50, 50, 255));
            }
        }
    }

    // 设置列宽
    connect(_tableWidget, &ElaTableWidget::tableWidgetShow, this, [=]() {
        _tableWidget->setColumnWidth(0, 60);
        _tableWidget->setColumnWidth(1, 205);
        _tableWidget->setColumnWidth(2, 170);
        _tableWidget->setColumnWidth(3, 150);
        _tableWidget->setColumnWidth(4, 60);
    });

    // 可选: 浮动滚动条
    ElaScrollBar* tableWidgetFloatScrollBar = new ElaScrollBar(_tableWidget->verticalScrollBar(), _tableWidget);
    tableWidgetFloatScrollBar->setIsAnimation(true);

    // 布局
    QHBoxLayout* tableWidgetLayout = new QHBoxLayout();
    tableWidgetLayout->setContentsMargins(0, 0, 10, 0);
    tableWidgetLayout->addWidget(_tableWidget);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("ElaTableWidget");
    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget);
    centerVLayout->setContentsMargins(0, 0, 0, 0);
    centerVLayout->addWidget(tableText);
    centerVLayout->addSpacing(10);
    centerVLayout->addLayout(tableWidgetLayout);
    centerVLayout->addStretch();
    addCentralWidget(centralWidget, true, false, 0);
}

T_TableWidget::~T_TableWidget()
{
}
