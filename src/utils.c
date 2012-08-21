void terminate(char* message) {
  fprintf(stderr, "%s\n", message);
  exit(1);
}

void check_arguments(int argc, const char *argv[]) {
  if (argc == 1) {
    fprintf(stderr, "No arguments provided\n");
    exit(1);
  }

  if (argc < 3) { 
    fprintf(stderr, "Argument count mismatch. %d out of 3\n", argc);
    exit(1);
  }
}

unsigned int check_path(char *path) {
  struct stat st;
  int result;

  result = stat(path, &st);
  if (result < 0) {
    fprintf(stderr, "Path does not exist\n");
    return 1;
  }

  if (S_ISDIR(st.st_mode) != 1) { 
    fprintf(stderr, "Path is not a directory\n");
    return 1;
  }

  return 0;
}

char** pcre_get_substrings(char* subject, int* ovector, int stringcount) {
  char **results = (char**) malloc(stringcount);
  const char *buffer;

  for(int i=0; i<stringcount; i++) {
    pcre_get_substring(subject, ovector, stringcount, i, &(buffer));
    results[i] = strdup(buffer);
    pcre_free_substring(buffer);
  }
  return results;
}