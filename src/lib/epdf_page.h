#ifndef __EPDF_PAGE_H__
#define __EPDF_PAGE_H__

#include <Evas.h>
#include <Ecore_Data.h>

#include "epdf_forward.h"

Epdf_Page* epdf_page_new(const Epdf_Document* doc);

void epdf_page_delete(Epdf_Page* page);

void epdf_page_render(Epdf_Page* page, Evas_Object* o);

void epdf_page_render_slice(Epdf_Page *page,
        Evas_Object* o,
        int          x,
        int          y,
        int          w,
        int          h);

void epdf_page_page_set(Epdf_Page* page, int p);

int epdf_page_page_get(const Epdf_Page* page);

void epdf_page_size_get(const Epdf_Page* page, int* width, int* height);

void epdf_page_content_geometry_get(const Epdf_Page* page, int* x, int* y, int* width, int* height);

void epdf_page_scale_set(Epdf_Page* page,
        double hscale,
        double vscale);

void epdf_page_scale_get(const Epdf_Page* page,
        double* hscale,
        double* vscale);

#endif /* __EPDF_PAGE_H__ */
