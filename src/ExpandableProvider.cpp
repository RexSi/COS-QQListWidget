// Copyright for gaia.

#define DEBUG_LEVEL 0
#define LOG_TAG "ListWidgetGroup"

#include <gaia/base/gloger.h>
#include <gaia/base/String.h>
#include <gaia/base/Vector.h>
#include <gaia/core/Page.h>
#include <gaia/ui/ListItemLite.h>

#include "ExpandableProvider.h"

using namespace gaia::base;
using namespace gaia::core;
using namespace gaia::ui;

ExpandableProvider::ExpandableProvider(Page* pPage)
        : mpPage(pPage) {
    GLOGENTRY(LOG_TAG);
}

ExpandableProvider::~ExpandableProvider() {
    GLOGENTRY(LOG_TAG);
    for (size_t i = 0; i < mWidgetPool.size(); i++) {
        delete mWidgetPool[i];
    }
    mWidgetPool.clear();
}

void ExpandableProvider::onGroupExpanded(int32_t groupPosition) {
    GLOGENTRY(LOG_TAG);
}

void ExpandableProvider::onGroupCollapsed(int32_t groupPosition) {
    GLOGENTRY(LOG_TAG);
}

int32_t ExpandableProvider::getGroupCount() {
    GLOGENTRY(LOG_TAG);
    return 20;
}

int32_t ExpandableProvider::getChildrenCount(int32_t groupPosition) {
    GLOGENTRY(LOG_TAG);
    return 10*  (groupPosition + 1);
}

bool ExpandableProvider::hasStableIds() {
    GLOGENTRY(LOG_TAG);
    return true;
}

bool ExpandableProvider::isChildSelectable(int32_t groupPosition, int32_t childPosition) {
    GLOGENTRY(LOG_TAG);
    return true;
}

bool ExpandableProvider::areAllItemsEnabled() {
    GLOGENTRY(LOG_TAG);
    return true;
}

bool ExpandableProvider::isEmpty() {
    GLOGENTRY(LOG_TAG);
    return false;
}

int64_t ExpandableProvider::getGroupId(int32_t groupPosition) {
    GLOGENTRY(LOG_TAG);
    return groupPosition;
}

int64_t ExpandableProvider::getChildId(int32_t groupPosition, int32_t childPosition) {
    GLOGENTRY(LOG_TAG);
    return childPosition;
}

Widget* ExpandableProvider::createGroupItemWidget(int32_t groupPosition) {
    GLOGENTRY(LOG_TAG);
    ListItemLite* pItem = new ListItemLite(mpPage, ListItemLite::CENTERTITLETEXTPANEL, ListItemLite::END);
    mWidgetPool.append(pItem);
    CenterTitleTextPanel::Handler* pHandler = dynamic_cast<CenterTitleTextPanel::Handler* >(pItem->getHandler(0));
    pHandler->setTextSize(30);
    return pItem;
}

Widget* ExpandableProvider::getGroupItemWidget(int32_t groupPosition, bool isExpanded, Widget* convertView, Widget* parent) {
    GLOGENTRY(LOG_TAG);
    ListItemLite* pItem = dynamic_cast<ListItemLite* >(convertView);

    CenterTitleTextPanel::Handler* pHandler = dynamic_cast<CenterTitleTextPanel::Handler* >(pItem->getHandler(0));

    pHandler->setText(String::format("  Group %d", groupPosition));
    return convertView;
}

Widget* ExpandableProvider::createChildItemWidget(int32_t groupPosition, int32_t childPosition) {
    GLOGENTRY(LOG_TAG);
    ListItemLite* pItem = new ListItemLite(mpPage, ListItemLite::CENTERTITLETEXTPANEL, ListItemLite::END);
    mWidgetPool.append(pItem);

    CenterTitleTextPanel::Handler* pHandler = dynamic_cast<CenterTitleTextPanel::Handler* >(pItem->getHandler(0));
    pHandler->setTextSize(30);
    pItem->setBackgroundColor(0xffd3d3d3);
    return pItem;
}

Widget* ExpandableProvider::getChildItemWidget(int32_t groupPosition, int32_t childPosition, bool isLastChild, Widget* convertWidget, Widget* parent) {
    GLOGENTRY(LOG_TAG);
    ListItemLite* pItem = dynamic_cast<ListItemLite* >(convertWidget);

    CenterTitleTextPanel::Handler* pHandler = dynamic_cast<CenterTitleTextPanel::Handler* >(pItem->getHandler(0));
    pHandler->setText(String::format("Child %d", childPosition));
    return pItem;
}
