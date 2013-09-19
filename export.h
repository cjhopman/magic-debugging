#if defined(COMPONENT_BUILD)

#if defined(WIN32)
#if defined(MAGIC_IMPLEMENTATION)
#define MAGIC_EXPORT __declspec(dllexport)
#define MAGIC_EXPORT_PRIVATE __declspec(dllexport)
#else
#define MAGIC_EXPORT __declspec(dllimport)
#define MAGIC_EXPORT_PRIVATE __declspec(dllimport)
#endif  // defined(MAGIC_IMPLEMENTATION)

#else  // defined(WIN32)
#if defined(MAGIC_IMPLEMENTATION)
#define MAGIC_EXPORT __attribute__((visibility("default")))
#define MAGIC_EXPORT_PRIVATE __attribute__((visibility("default")))
#else
#define MAGIC_EXPORT
#define MAGIC_EXPORT_PRIVATE
#endif  // defined(MAGIC_IMPLEMENTATION)
#endif

#else // COMPONENT_BUILD
#define MAGIC_EXPORT
#define MAGIC_EXPORT_PRIVATE
#endif // COMPONENT_BUILD

