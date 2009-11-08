#include <fitz.h>
#include <mupdf.h>

#include "epdf_enum.h"
#include "epdf_private.h"
#include "Epdf.h"

static void epdf_index_fill(Ecore_List* items, pdf_outline* entry);
static void epdf_index_unfill(Ecore_List* items);

/* Index item */
Epdf_Index_Item* epdf_index_item_new()
{
    Epdf_Index_Item* item;

    item = (Epdf_Index_Item*)malloc(sizeof(Epdf_Index_Item));
    if(!item)
        return NULL;

    item->title = NULL;
    item->link = NULL;
    item->children = NULL;

    return item;
}

void epdf_index_item_delete(Epdf_Index_Item* item)
{
    if(!item)
        return;

    if(item->title)
        free(item->title);
    if(item->children)
    {
        Epdf_Index_Item* i;

        ecore_list_first_goto(item->children);
        while((i = (Epdf_Index_Item*)ecore_list_next(item->children)))
            epdf_index_item_delete(i);
    }
    free(item);
}

const char* epdf_index_item_title_get(const Epdf_Index_Item* item)
{
    if(!item)
        return NULL;

    return item->title;
}

Ecore_List* epdf_index_item_children_get(const Epdf_Index_Item* item)
{
    if(!item)
        return NULL;

    return item->children;
}

Epdf_Link_Action_Kind epdf_index_item_action_kind_get(const Epdf_Index_Item* item)
{
    if(!item || !item->link)
        return EPDF_LINK_ACTION_UNKNOWN;

    if(PDF_LURI == item->link->kind)
        return EPDF_LINK_ACTION_URI;
    else if(PDF_LGOTO == item->link->kind)
        return EPDF_LINK_ACTION_GOTO;
}

int epdf_index_item_page_get(const Epdf_Document* doc, const Epdf_Index_Item* item)
{
    bool delete_dest = false;

    if(!item || !item->link)
        return -1;

    if(PDF_LGOTO != item->link->kind)
        return -1;

    fz_obj* dest = item->link->dest;
    int p = 0;
    if(fz_isint(dest))
    {
        p = fz_toint(dest);
        return p;
    }
    if(fz_isdict(dest))
    {
        // The destination is linked from a Go-To action's D array
        fz_obj* D = fz_dictgets(dest, "D");
        if(D && fz_isarray(D))
            dest = fz_arrayget(D, 0);
    }
    int n = fz_tonum(dest);
    int g = fz_togen(dest);

    for(p = 1; p <= epdf_document_page_count_get(doc); p++)
    {
        fz_obj* page;
        fz_error error = pdf_getpageobject(doc->xref, p, &page);
        if(error)
            continue;
        int np = fz_tonum(page);
        int gp = fz_togen(page);
        if(n == np && g == gp)
            return p-1;
    }

    return 0;
}

/* Index */

Ecore_List* epdf_index_new(const Epdf_Document* doc)
{
    Ecore_List* index = NULL;

    if(!doc)
        return index;

    if(doc->outline == NULL)
        return index;

    index = ecore_list_new();
    epdf_index_fill(index, doc->outline);

    return index;
}

void epdf_index_delete(Ecore_List* index)
{
    Ecore_List* items = index;
    Epdf_Index_Item* item;

    if(!index)
        return;

    epdf_index_unfill(index);
}

static void epdf_index_fill(Ecore_List* items, pdf_outline* entry)
{
    if(!items || !entry)
        return;

    Epdf_Index_Item* item;
    item = epdf_index_item_new();
    item->title = entry->title;
    item->link = entry->link;

    ecore_list_append(items, item);

    if(entry->child)
    {
        item->children = ecore_list_new();
        epdf_index_fill(item->children, entry->child);
    }

    if(entry->next)
        epdf_index_fill(items, entry->next);
}

static void epdf_index_unfill(Ecore_List* items)
{
    Epdf_Index_Item* item;

    if(!items)
        return;

    ecore_list_first_goto(items);
    while((item = (Epdf_Index_Item*)ecore_list_next(items)))
    {
        /*
         * if (item->title)
         * free (item->title);
         * if (item->children)
         * epdf_index_unfill (item->children);
         */
        free (item);
    }
    ecore_list_destroy (items);
}
