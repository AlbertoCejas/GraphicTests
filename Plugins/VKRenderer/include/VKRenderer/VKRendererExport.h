
#ifndef VKRENDERER_EXPORT_H
#define VKRENDERER_EXPORT_H

#ifdef VKRENDERER_STATIC
#  define VKRENDERER_EXPORT
#  define VKRENDERER_NO_EXPORT
#else
#  ifndef VKRENDERER_EXPORT
#    ifdef VKRenderer_EXPORTS
        /* We are building this library */
#      define VKRENDERER_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VKRENDERER_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VKRENDERER_NO_EXPORT
#    define VKRENDERER_NO_EXPORT 
#  endif
#endif

#ifndef VKRENDERER_DEPRECATED
#  define VKRENDERER_DEPRECATED __declspec(deprecated)
#endif

#ifndef VKRENDERER_DEPRECATED_EXPORT
#  define VKRENDERER_DEPRECATED_EXPORT VKRENDERER_EXPORT VKRENDERER_DEPRECATED
#endif

#ifndef VKRENDERER_DEPRECATED_NO_EXPORT
#  define VKRENDERER_DEPRECATED_NO_EXPORT VKRENDERER_NO_EXPORT VKRENDERER_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef VKRENDERER_NO_DEPRECATED
#    define VKRENDERER_NO_DEPRECATED
#  endif
#endif

#endif /* VKRENDERER_EXPORT_H */
