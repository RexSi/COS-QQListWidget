// Copyright for gaia.

#pragma once

#include <gaia/ui/AbsExpandableListProvider.h>

class ExpandableProvider: public gaia::ui::AbsExpandableListProvider {
public:
    ExpandableProvider(gaia::core::Page* pPage);
    virtual ~ExpandableProvider();

    virtual void onGroupExpanded(int32_t groupPosition);
    virtual void onGroupCollapsed(int32_t groupPosition);
    virtual int32_t getGroupCount();
    virtual int32_t getChildrenCount(int32_t groupPosition);
    virtual bool hasStableIds();
    virtual bool isChildSelectable(int32_t groupPosition, int32_t childPosition);
    virtual bool areAllItemsEnabled();
    virtual bool isEmpty();
    virtual int64_t getGroupId(int32_t groupPosition);
    virtual int64_t getChildId(int32_t groupPosition, int32_t childPosition);
    virtual gaia::core::Widget *createGroupItemWidget(int32_t groupPosition);
    virtual gaia::core::Widget* getGroupItemWidget(int32_t groupPosition, bool isExpanded, gaia::core::Widget* convertView, gaia::core::Widget* parent);
    virtual gaia::core::Widget* createChildItemWidget(int32_t groupPosition, int32_t childPosition);
    virtual gaia::core::Widget* getChildItemWidget(int32_t groupPosition, int32_t childPosition, bool isLastChild, gaia::core::Widget *convertWidget, gaia::core::Widget *parent);

private:
    gaia::core::Page *mpPage;
    gaia::base::Vector<gaia::core::Widget*> mWidgetPool;
};

