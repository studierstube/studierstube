#ifdef WIN32
  // somehow ::TryEnterCriticalSection() is not declared in release build...
  // (needed by ACE)
  BOOL TryEnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
#endif //WIN32
