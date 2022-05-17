
#ifndef GLRENDERER_EXPORT_H
#define GLRENDERER_EXPORT_H

#ifdef GLRENDERER_STATIC
#  define GLRENDERER_EXPORT
#  define GLRENDERER_NO_EXPORT
#else
#  ifndef GLRENDERER_EXPORT
#    ifdef GLRenderer_EXPORTS
        /* We are building this library */
#      define GLRENDERER_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define GLRENDERER_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef GLRENDERER_NO_EXPORT
#    define GLRENDERER_NO_EXPORT 
#  endif
#endif

#ifndef GLRENDERER_DEPRECATED
#  define GLRENDERER_DEPRECATED __declspec(deprecated)
#endif

#ifndef GLRENDERER_DEPRECATED_EXPORT
#  define GLRENDERER_DEPRECATED_EXPORT GLRENDERER_EXPORT GLRENDERER_DEPRECATED
#endif

#ifndef GLRENDERER_DEPRECATED_NO_EXPORT
#  define GLRENDERER_DEPRECATED_NO_EXPORT GLRENDERER_NO_EXPORT GLRENDERER_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef GLRENDERER_NO_DEPRECATED
#    define GLRENDERER_NO_DEPRECATED
#  endif
#endif

#endif /* GLRENDERER_EXPORT_H */
