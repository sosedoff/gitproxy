void get_session(g_session *session) {
  session->user           = getenv("USER");
  session->home           = getenv("HOME");
  session->ssh_client     = getenv("SSH_CLIENT");
  session->ssh_connection = getenv("SSH_CONNECTION");
  session->ssh_command    = getenv("SSH_ORIGINAL_COMMAND");
}

g_request* get_request(char* command) {
  const char *error;
  int error_offset;
  int num = 3;
  int ovector[num*3];
  int result;
  int options = 0;
  pcre *regex;
  g_request *request;

  regex = pcre_compile(REGEX_GIT_COMMAND, options, &error, &error_offset, 0);
  if (!regex) {
    fprintf(stderr, "PCRE failed (offset: %d), %s\n", error_offset, error);
    return NULL;
  }

  result = pcre_exec(regex, 0, command, strlen(command), 0, 0, ovector, num*3);
  if (result <= 0) {
    switch(result) {
      case PCRE_ERROR_NOMATCH      : fprintf(stderr, "String did not match the pattern\n");        break;
      case PCRE_ERROR_NULL         : fprintf(stderr, "Something was null\n");                      break;
      case PCRE_ERROR_BADOPTION    : fprintf(stderr, "A bad option was passed\n");                 break;
      case PCRE_ERROR_BADMAGIC     : fprintf(stderr, "Magic number bad (compiled re corrupt?)\n"); break;
      case PCRE_ERROR_UNKNOWN_NODE : fprintf(stderr, "Something kooky in the compiled re\n");      break;
      case PCRE_ERROR_NOMEMORY     : fprintf(stderr, "Ran out of memory\n");                       break;
      default                      : fprintf(stderr, "Unknown error\n");                           break;
    }
  }

  if (result > 0) {
    char **results = pcre_get_substrings(command, ovector, result);
    request = (g_request*) malloc(sizeof(g_request));

    request->command  = results[0];
    request->action   = results[1];
    request->repo     = results[2];
    request->is_read  = 0;
    request->is_write = 0;

    request->repo_path = (char*) malloc(255);
    sprintf(request->repo_path, "%s/%s", GIT_ROOT, request->repo);

    if (strcmp(request->action, "git-upload-pack") == 0 ||
        strcmp(request->action, "git upload-pack") == 0) {
      request->is_read = 1;
    }

    if (strcmp(request->action, "git-receive-pack") == 0 ||
        strcmp(request->action, "git receive-pack") == 0) {
      request->is_write = 1;
    }
  }

  pcre_free(regex);
  return request;
}

unsigned int validate_session(g_session session) {
  if (GITPROXY_DEBUG == 1) {
    fprintf(stderr, "Session:\n");
    fprintf(stderr, " -> User: %s\n",           session.user);
    fprintf(stderr, " -> Home: %s\n",           session.home);
    fprintf(stderr, " -> SSH Client: %s\n",     session.ssh_client);
    fprintf(stderr, " -> SSH Connection: %s\n", session.ssh_connection);
    fprintf(stderr, " -> SSH Command: %s\n",    session.ssh_command);
  }
  
  if (strcmp(session.user, GIT_USER) != 0) {
    fprintf(stderr, "Error: Invalid user => %s\n", session.user);
    return 1;
  }
  
  if (strcmp(session.home, GIT_HOME) != 0) {
    fprintf(stderr, "Error: Invalid home => %s\n", session.home);
    return 1;
  }

  if (session.ssh_command == NULL) {
    fprintf(stderr, "Error: Not a git command\n");
    return 1;
  }

  return 0;
}