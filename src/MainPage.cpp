// Copyright for gaia.

#define DEBUG_LEVEL 0
#define LOG_TAG "ListWidgetGroup"

#include <com/gaia/R_SDK.h>
#include <com/gaia/common/R_SDK.h>
#include <gaia/base/gloger.h>
#include <gaia/base/HashMap.h>
#include <gaia/base/String.h>
#include <gaia/base/Vector.h>
#include <gaia/core/Context.h>
#include <gaia/core/Persistence.h>
#include <gaia/core/vision/Scene.h>
#include <gaia/ui/ExpandableListWidget.h>
#include <gaia/ui/MarginControllerParams.h>
#include <gaia/ui/RelativeController.h>
#include <gaia/ui/ListItemLite.h>
#include <gaia/ui/TextWidget.h>
#include <ListWidgetGroup/R.h>

#include "ExpandableProvider.h"
#include "MainPage.h"

using namespace gaia::base;
using namespace gaia::core;
using namespace gaia::ui;

class MyOnGroupItemClickListener: public ExpandableListWidget::OnGroupItemClickListener {
public:
    explicit MyOnGroupItemClickListener(TextWidget *pText)
            : mpText(pText) {
    }

    virtual bool onGroupClick(ExpandableListWidget *parent, Widget *pWidget, int32_t groupPosition, int64_t id) {
        GLOG(LOG_TAG, LOGINFO, "group item: %d(addr: %p, ID:%lld) is clicked", groupPosition, pWidget, id);
        if (mpText) {
            mpText->setText(String::format("group item: %d", groupPosition));
        }
        return true;
    }

private:
    TextWidget *mpText;
};

class MyOnChildItemClickListener: public ExpandableListWidget::OnChildItemClickListener {
public:
    explicit MyOnChildItemClickListener(TextWidget *pText)
            : mpText(pText) {
    }

    virtual bool onChildClick(ExpandableListWidget *parent, Widget *pWidget, int32_t groupPosition, int32_t childPosition, int64_t id) {
        GLOG(LOG_TAG, LOGINFO, "Child item: %d(group: %d, addr: %p" ", ID:%lld) is clicked", childPosition, groupPosition, pWidget, id);
        if (mpText) {
            mpText->setText(String::format("Child item: %d is clicked", childPosition));
        }
        return true;
    }

private:
    TextWidget *mpText;
};

MainPage::MainPage()
        : mpListWidget(NULL),
          mpProvider(NULL),
          mIndicatorGroupHeight(0),
          mTheGroupExpandPosition(-1),
          mCountExpand(0),
          mpTop(NULL),
          mpMarginParams(NULL) {
    GLOGENTRY(LOG_TAG);
}

MainPage::~MainPage() {
    GLOGENTRY(LOG_TAG);
}

void MainPage::onInit(Persistence* const p) {
    GLOGENTRY(LOG_TAG);
    Scene::SceneFactory(this);

    RelativeController* pRc = new RelativeController(this);
    mWidgetPool.append(pRc);

    mpMarginParams = new MarginControllerParams(ControllerParams::MATCH_PARENT, ControllerParams::WRAP_CONTENT);

    mpTop = new ListItemLite(this, ListItemLite::CENTERTITLETEXTPANEL, ListItemLite::END);
    mWidgetPool.append(mpTop);
    mpTop->setControllerParams(mpMarginParams);

    mpListWidget = new ExpandableListWidget(this);
    mpListWidget->setIndicatorBounds(0, 0);

    mpListWidget->setOnGroupCollapseListener(this);
    mpListWidget->setOnGroupExpandListener(this);
    mpListWidget->setOnScrollListener(this);

    mpProvider = new ExpandableProvider(this);
    mpProvider->setAutoClearWidgetEnabled(true);

    mpListWidget->setProvider(mpProvider);
    CenterTitleTextPanel::Handler* pHandler = dynamic_cast<CenterTitleTextPanel::Handler*>(mpTop->getHandler(0));
    pHandler->setTextSize(30);

    mpTop->setOnClickListener(this);

    pRc->addWidget(mpListWidget);
    pRc->addWidget(mpTop);

    mpScene->attachController(pRc);
    mpScene->setupSceneLayout();
}

void MainPage::onTear() {
    GLOGENTRY(LOG_TAG);
    for (size_t i = 0; i < mWidgetPool.size(); i++) {
        delete mWidgetPool.itemAt(i);
    }
    mWidgetPool.clear();
    delete mpListWidget;
    delete mpProvider;
    delete mpMarginParams;
    getSceneSingleton()->SceneDestroy();
}

void MainPage::onClick(Widget* w) {
    GLOGENTRY(LOG_TAG);
    mpTop->setVisibility(Widget::GONE);
    mpListWidget->collapseGroup(mTheGroupExpandPosition);
    mpListWidget->setSelectedGroup(mTheGroupExpandPosition);
}

void MainPage::onScrollStateChanged(AbsListWidget *pWidget, int32_t scrollState) {
    GLOGENTRY(LOG_TAG);
}

void MainPage::onScroll(AbsListWidget* pWidget, int32_t firstVisibleItem, int32_t visibleItemCount, int32_t totalItemCount) {
    GLOGENTRY(LOG_TAG);
    //防止三星,魅族等手机第一个条目可以一直往下拉,父条目和悬浮同时出现的问题
    if (firstVisibleItem == 0) {
        mpTop->setVisibility(Widget::GONE);
    }
    // 控制滑动时TextView的显示与隐藏
    int32_t npos = pWidget->pointToPosition(0, 0);
    GLOG(LOG_TAG, LOGINFO, "npos: %d", npos);
    if (npos != AbsListWidget::INVALID_POSITION) {
        int64_t pos = mpListWidget->getExpandableListPosition(npos);
        int32_t childPos = ExpandableListWidget::getPackedPositionChild(pos);
        const int32_t groupPos = ExpandableListWidget::getPackedPositionGroup(pos);
        GLOG(LOG_TAG, LOGINFO, "pos: %lld, childPos %d, groupPos %d", pos, childPos, groupPos);
        if (childPos == AbsListWidget::INVALID_POSITION) {
            Widget* groupView = mpListWidget->getChildWidget(npos - mpListWidget->getFirstVisiblePosition());
            mIndicatorGroupHeight = groupView->getHeight();
            GLOG(LOG_TAG, LOGINFO, "mIndicatorGroupHeight: %d", mIndicatorGroupHeight);
        }

        if (mIndicatorGroupHeight == 0) {
            return;
        }
        // if (isExpanded) {
        if (mCountExpand > 0) {
            mTheGroupExpandPosition = groupPos;

            GLOG(LOG_TAG, LOGINFO, "mCountExpand: %d", mCountExpand);
            CenterTitleTextPanel::Handler* pHandler = dynamic_cast<CenterTitleTextPanel::Handler*>(mpTop->getHandler(0));
            pHandler->setText(String::format("  Group %d", mTheGroupExpandPosition));

            if (mTheGroupExpandPosition != groupPos || !mpListWidget->isGroupExpanded(groupPos)) {
                mpTop->setVisibility(Widget::GONE);
                GLOG(LOG_TAG, LOGINFO, "mTheGroupExpandPosition: %d", mTheGroupExpandPosition);

            } else {
                mpTop->setVisibility(Widget::VISIBLE);
                GLOG(LOG_TAG, LOGINFO, "mTheGroupExpandPosition: %d", mTheGroupExpandPosition);
            }
        }
        if (mCountExpand == 0) {
            mpTop->setVisibility(Widget::GONE);
        }
    }

    if (mTheGroupExpandPosition == -1) {
        return;
    }
    /**
     * calculate point (0,indicatorGroupHeight)
     */
    int32_t showHeight = getHeight();
    // 得到悬浮的条滑出屏幕多少
    mpMarginParams->setMargin(MarginControllerParams::TOP, -(mIndicatorGroupHeight - showHeight));
}

int32_t MainPage::getHeight() {
    GLOGENTRY(LOG_TAG);
    int32_t showHeight = mIndicatorGroupHeight;
    int32_t nEndPos = mpListWidget->pointToPosition(0, mIndicatorGroupHeight);
    if (nEndPos != AbsListWidget::INVALID_POSITION) {
        int64_t pos = mpListWidget->getExpandableListPosition(nEndPos);
        int32_t groupPos = ExpandableListWidget::getPackedPositionGroup(pos);
        if (groupPos != mTheGroupExpandPosition) {
            Widget* viewNext = mpListWidget->getChildWidget(nEndPos - mpListWidget->getFirstVisiblePosition());
            showHeight = viewNext->getTop();
        }
    }
    return showHeight;
}

void MainPage::onGroupCollapse(int32_t groupPosition) {
    GLOGENTRY(LOG_TAG);
    mGroupIds.remove(groupPosition);
    mCountExpand = mGroupIds.size();
}

void MainPage::onGroupExpand(int32_t groupPosition) {
    GLOGENTRY(LOG_TAG);
    mTheGroupExpandPosition = groupPosition;
    mGroupIds.add(groupPosition, 0);
    mCountExpand = mGroupIds.size();
}

template class Export<MainPage, Page> ;

