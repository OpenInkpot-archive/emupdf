#ifndef __EPDF_DOCUMENT_H__
#define __EPDF_DOCUMENT_H__

Epdf_Document* epdf_document_new(const char* filename);
void epdf_document_delete(Epdf_Document* document);
int epdf_document_page_count_get(const Epdf_Document* document);
unsigned char epdf_document_is_locked(const Epdf_Document* doc);
unsigned char epdf_document_unlock(Epdf_Document* doc, const char* password);

char* epdf_document_title_get(const Epdf_Document* doc);
char* epdf_document_author_get(const Epdf_Document* doc);
char* epdf_document_subject_get(const Epdf_Document* doc);
char* epdf_document_keywords_get(const Epdf_Document* doc);
char* epdf_document_creator_get(const Epdf_Document* doc);
char* epdf_document_producer_get(const Epdf_Document* doc);
char* epdf_document_creation_date_get(const Epdf_Document* doc);
char* epdf_document_mod_date_get(const Epdf_Document* doc);

#endif /* __EPDF_DOCUMENT_H__ */
