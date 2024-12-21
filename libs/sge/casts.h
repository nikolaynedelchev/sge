#pragma once
#include "engine_bridge.h"
#include <raylib.h>

namespace ndn::sge
{

#define DD_CAST_TYPES_LIST(do_macro)                                     \
    do_macro(   Vector2                 ,   ::ndn::sge::point                 )\
    do_macro(   Rectangle               ,   ::ndn::sge::rect                  )\
    do_macro(   Color                   ,   ::ndn::sge::color                 )\
    do_macro(   Image                   ,   ::ndn::sge::image                 )\
    do_macro(   Texture2D               ,   ::ndn::sge::texture               )\
    do_macro(   AudioStream             ,   ::ndn::sge::audio_stream          )\
    do_macro(   Sound                   ,   ::ndn::sge::sound_impl            )\
    do_macro(   Music                   ,   ::ndn::sge::music_impl            )\
    do_macro(   KeyboardKey             ,   ::ndn::sge::keys::kbd_key         )\
    do_macro(   GlyphInfo               ,   ::ndn::sge::glyph_info            )\
    do_macro(   Font                    ,   ::ndn::sge::font                  )\

//
#define DD_TYPE_CAST(ray_, dd_) \
static inline ray_ cast(const dd_ & val) { return reinterpret_cast< const ray_ & >(val); } \
static inline dd_  cast(const ray_ & val) { return reinterpret_cast< const dd_ & >(val); } \

DD_CAST_TYPES_LIST( DD_TYPE_CAST )

}
