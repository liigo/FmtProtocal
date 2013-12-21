#ifndef __BUF_FMT_PARSER_H__
#define __BUF_FMT_PARSER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "../protocol/protocol.h"
#include <stdbool.h>

//in the callback, you can get a FMT* by buffered_fmt_parser_take(parser)
typedef void (*fmt_available_callback)(short int cmd, void* userdata);

void* buffered_fmt_parser_new(bool usedForServer);
void buffered_fmt_parser_delete(void* parser);
void buffered_fmt_parser_push(void* parser, const void* data, unsigned int len,
							fmt_available_callback cb, void* userdata);
bool buffered_fmt_parser_complete(void* parser);
FMT* buffered_fmt_parser_take(void* parser); //need fmt_object_put(fmt)
short int buffered_fmt_parser_cmd(void* parser);
void buffered_fmt_parser_reset(void* parser);

#ifdef __cplusplus
}
#endif

#endif //__BUF_FMT_PARSER_H__
