#ifndef __EPDF_INDEX_H__
#define __EPDF_INDEX_H__

#include <Eina.h>
#include "epdf_forward.h"

Epdf_Index_Item * epdf_index_item_new ();
const char *epdf_index_item_title_get (const Epdf_Index_Item *item);
Eina_List *epdf_index_item_children_get (const Epdf_Index_Item *item);
Epdf_Link_Action_Kind epdf_index_item_action_kind_get (const Epdf_Index_Item *item);
int epdf_index_item_page_get (const Epdf_Document *document, const Epdf_Index_Item *item);
Eina_List *epdf_index_new (const Epdf_Document *document);
void epdf_index_delete (Eina_List *index);

#endif /* __EPDF_INDEX_H__ */
