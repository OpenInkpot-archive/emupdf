#include <Evas.h>
#include <Ecore_Data.h>

#include <fitz.h>
#include <mupdf.h>

#include "epdf_private.h"
#include "Epdf.h"

Epdf_Page* epdf_page_new(const Epdf_Document* doc)
{
    Epdf_Page* page;

    if(!doc)
        return NULL;

    page = (Epdf_Page*)malloc(sizeof(Epdf_Page));
    if(!page)
        return NULL;

    page->doc = (Epdf_Document*)doc;
    page->page = NULL;
    page->index = 0;
    page->image = NULL;

    return page;
}

void epdf_page_delete(Epdf_Page* page)
{
    if(!page)
        return;

    if(page->image)
    {
        fz_droppixmap(page->image);
        page->image = NULL;
    }

    if(page->page)
    {
        pdf_droppage(page->page); 
        page->page = NULL;
    }

    free(page);
}

static fz_matrix epdf_page_viewctm(Epdf_Page* page)
{
    Epdf_Document* doc = page->doc;

    fz_matrix ctm;
    ctm = fz_identity();
    ctm = fz_concat(ctm, fz_translate(0, -page->page->mediabox.y1));
    ctm = fz_concat(ctm, fz_scale(doc->zoom, -doc->zoom));
    ctm = fz_concat(ctm, fz_rotate(doc->rotate + page->page->rotate));
    return ctm;
}

static void epdf_page_load(Epdf_Page* page)
{
    fz_error error;
    fz_obj* obj;

    if(!page)
        return;

    if(page->page)
        pdf_droppage(page->page);
    page->page = nil;

    Epdf_Document* doc = page->doc;
    if(!doc)
        return;

    // load page
    error = pdf_getpageobject(doc->xref, page->index, &obj);
    if(error)
        return;

    error = pdf_loadpage(&page->page, doc->xref, obj);
    if(error)
        return;
}

void epdf_page_render(Epdf_Page* page, Evas_Object* o)
{
    epdf_page_render_slice(page, o, 0, 0, -1, -1);
}

void epdf_page_render_slice(Epdf_Page* page,
        Evas_Object* o,
        int          x,
        int          y,
        int          w,
        int          h)
{
    fz_error error;
    fz_matrix ctm;
    fz_rect bbox;
    fz_obj* obj;

    if(!page)
        return;

    if(!page->page)
        epdf_page_load(page);

    Epdf_Document* doc = page->doc;
    if(!doc)
        return;

    // draw page
    if(page->image)
        fz_droppixmap(page->image);
    page->image = nil;

    ctm = epdf_page_viewctm(page);

    if(w < 0 || h < 0)
    {
        bbox = fz_transformaabb(ctm, page->page->mediabox);
    }
    else
    {
        bbox.x0 = x;
        bbox.x1 = x + w;
        bbox.y0 = y;
        bbox.y1 = y + h;
    }

    error = fz_rendertree(&page->image, doc->rast, page->page->tree,
            ctm, fz_roundrect(bbox), 1);
    if(error)
        return;

    int width = page->image->w;
    int height = page->image->h;

    /*evas_object_image_data_set(o, page->image->samples);
      evas_object_image_fill_set(o, 0, 0, width, height);
      evas_object_image_size_set(o, width, height);
      evas_object_image_data_update_add(o, 0, 0, width, height);
      evas_object_resize(o, width, height);
      */

    evas_object_image_size_set(o, width, height);
    evas_object_image_fill_set(o, 0, 0, width, height);

    //memcpy(m, page->image->samples, height * width * 4);
    unsigned char* s = page->image->samples;
    unsigned char* d = (unsigned char*)evas_object_image_data_get(o, 1);
    for(int i = 0; i < height * width; i++, s+=4, d+=4)
    {
        d[0] = s[3];
        d[1] = s[2];
        d[2] = s[1];
        d[3] = s[0];
    }

    if(page->image)
        fz_droppixmap(page->image);
    page->image = nil;

    evas_object_image_data_update_add(o, 0, 0, width, height);
    evas_object_resize(o, width, height);
}

void epdf_page_page_set(Epdf_Page* page, int p)
{
    if(!page)
        return;

    page->index = p + 1;
    if(page->page)
        pdf_droppage(page->page);
    page->page = nil;
}

int epdf_page_page_get(const Epdf_Page* page)
{
    if(!page)
        return -1;

    return page->index - 1;
}

void epdf_page_size_get(const Epdf_Page* page, int* width, int* height)
{
    if(!page)
    {
        *width = 0;
        *height = 0;
        return;
    }

    if(!page->page)
        epdf_page_load((Epdf_Page*)page);

    fz_rect mediabox = page->page->mediabox;

    *width = mediabox.x1 - mediabox.x0;
    *height = mediabox.y1 - mediabox.y0;
}

void epdf_page_content_geometry_get(const Epdf_Page* page, int* x, int* y, int* width, int* height)
{
    if(!page)
    {
        *x = 0;
        *y = 0;
        *width = 0;
        *height = 0;
        return;
    }

    if(!page->page)
        epdf_page_load((Epdf_Page*)page);

    fz_rect contentbox = fz_boundnode(page->page->tree->root, fz_identity());
    fz_rect b = contentbox;

    *x = contentbox.x0;
    *y = contentbox.y0;
    *width = contentbox.x1 - contentbox.x0;
    *height = contentbox.y1 - contentbox.y0;
}

void epdf_page_scale_set(Epdf_Page* page,
        double hscale,
        double vscale)
{
    if(!page || !page->doc)
        return;

    page->doc->zoom = hscale;
}

void epdf_page_scale_get(const Epdf_Page* page,
        double* hscale,
        double* vscale)
{
    if(!page || !page->doc)
    {
        *hscale = 0.0;
        *vscale = 0.0;
        return;
    }

    *hscale = *vscale = page->doc->zoom;
}
