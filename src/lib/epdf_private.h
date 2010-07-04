#ifndef __EPDF_PRIVATE_H__
#define __EPDF_PRIVATE_H__

#include <stdbool.h>

#include <Eina.h>

#include <mupdf.h>
#include <fitz.h>

#include "epdf_forward.h"

struct _Epdf_Page
{
    Epdf_Document        *doc;
    int                   index;
    pdf_page             *page;
    fz_pixmap            *image;
};

struct _Epdf_Document
{
    char            *filename;
    char            *doctitle;
    pdf_xref        *xref;
    pdf_outline     *outline;
    fz_renderer     *rast;
    int              pagecount;
    float            zoom;
    int              rotate;
    bool             locked;
};

struct _Epdf_Index_Item
{
    char       *title;
    pdf_link   *link;
    Eina_List *children;
};

#endif /* __EPDF_PRIVATE_H__ */
