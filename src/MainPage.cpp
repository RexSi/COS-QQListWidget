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
#include <gaia/ui/RelativeControllerParams.h>
#include <gaia/ui/RelativeController.h>
#include <gaia/ui/ListItemLite.h>
#include <gaia/ui/TextWidget.h>
#include <ListWidgetGroup/R.h>

#include "ExpandableProvider.h"
#include "MainPage.h"

using namespace gaia::base;
using namespace gaia::core;
using namespace gaia::ui;

MainPage::MainPage()
        : mpListWidget(NULL),
          mpProvider(NULL),
          mIndicatorGroupHeight(0),
          mTheGroupExpandPosition(-1),
          mCountExpand(0),
          mpTop(NULL),
          mpRelativeControllerParams(NULL) {
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

    mpRelativeControllerParams = new RelativeControllerParams(ControllerParams::MATCH_PARENT, ControllerParams::WRAP_CONTENT);
    mpRelativeControllerParams->setRelation(RelativeControllerParams::ALIGN_PARENT_TOP);

    mpTop = new ListItemLite(this, ListItemLite::CENTERTITLETEXTPANEL, ListItemLite::END);
    mWidgetPool.append(mpTop);
    mpTop->setControllerParams(mpRelativeControllerParams);

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
    delete mpRelativeControllerParams;
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
    if (firstVisibleItem == 0) {
        mpTop->setVisibility(Widget::GONE);
    }

    if (mCountExpand <= 0) {
        mpTop->setVisibility(Widget::GONE);
        return;
    }
    int32_t npos = pWidget->pointToPosition(0, 0);
    if (npos == AbsListWidget::INVALID_POSITION) {
        return;
    }
    const int64_t pos = mpListWidget->getExpandableListPosition(npos);
    const int32_t childPos = ExpandableListWidget::getPackedPositionChild(pos);
    const int32_t groupPos = ExpandableListWidget::getPackedPositionGroup(pos);
    if (!mpListWidget->isGroupExpanded(groupPos)) {
        mpTop->setVisibility(Widget::GONE);
        return;
    }
    mTheGroupExpandPosition = groupPos;

    if (mTheGroupExpandPosition == -1) {
        return;
    }

    if (childPos == AbsListWidget::INVALID_POSITION) {
        Widget* groupWidget = mpListWidget->getChildWidget(npos - mpListWidget->getFirstVisiblePosition());
        mIndicatorGroupHeight = groupWidget->getHeight();
        GLOG(LOG_TAG, LOGINFO, "mIndicatorGroupHeight: %d", mIndicatorGroupHeight);
    }

    if (mIndicatorGroupHeight == 0) {
        return;
    }

    mpTop->setVisibility(Widget::VISIBLE);
    CenterTitleTextPanel::Handler* pHandler = dynamic_cast<CenterTitleTextPanel::Handler*>(mpTop->getHandler(0));
    pHandler->setText(String::format("  Group %d", mTheGroupExpandPosition));

    int32_t showHeight = getHeight();
    if (showHeight == 0) {
        mpTop->setVisibility(Widget::GONE);
        return;
    }
    mpRelativeControllerParams->setMargin(MarginControllerParams::TOP, -(mIndicatorGroupHeight - showHeight));
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
    mGroupIds.remove(groupPosition);
    mCountExpand = mGroupIds.size();
}

void MainPage::onGroupExpand(int32_t groupPosition) {
    mTheGroupExpandPosition = groupPosition;
    mGroupIds.add(groupPosition, 0);
    mCountExpand = mGroupIds.size();
}

template class Export<MainPage, Page> ;

