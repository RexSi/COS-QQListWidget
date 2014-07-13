// Copyright for gaia.

#pragma once

#include <gaia/core/event/OnClickListener.h>
#include <gaia/core/Page.h>
#include <gaia/ui/AbsListWidget.h>

class MainPage: public gaia::core::Page,
        public gaia::ui::AbsListWidgetOnScrollListener,
        public gaia::ui::ExpandableListWidget::OnGroupItemCollapseListener,
        public gaia::ui::ExpandableListWidget::OnGroupItemExpandListener,
        public gaia::core::OnClickListener {
public:
    MainPage();
    ~MainPage();

protected:
    virtual void onInit(gaia::core::Persistence* const p);
    virtual void onTear();

    virtual void onScrollStateChanged(gaia::ui::AbsListWidget *pWidget, int32_t scrollState);
    virtual void onScroll(gaia::ui::AbsListWidget *pWidget, int32_t firstVisibleItem, int32_t visibleItemCount, int32_t totalItemCount);

    virtual void onGroupCollapse(int32_t groupPosition);
    virtual void onGroupExpand(int32_t groupPosition);

    virtual void onClick(gaia::core::Widget* w);

private:
    int32_t getHeight();

private:
    gaia::ui::ExpandableListWidget* mpListWidget;
    gaia::ui::AbsExpandableListProvider* mpProvider;
    int32_t mIndicatorGroupHeight;
    int32_t mTheGroupExpandPosition;
    int32_t mCountExpand;
    gaia::ui::ListItemLite* mpTop;
    gaia::ui::RelativeControllerParams* mpRelativeControllerParams;
    gaia::base::HashMap<int32_t, int32_t> mGroupIds;
    gaia::base::Vector<gaia::core::Widget*> mWidgetPool;
};
