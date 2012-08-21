#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <pcre.h>

#include "include/gitproxy.h"
#include "src/utils.c"
#include "src/gitproxy.c"

int main(int argc, const char *argv[]) {
  check_arguments(argc, argv);

  const char *app_key  = argv[1];
  const char *app_user = argv[2];
  int file_stat;
  g_session session;
  g_request *request;
  
  get_session(&session);
  if (validate_session(session) != 0) { 
    terminate("Not a git session!\n"); 
  }
  
  request = get_request(session.ssh_command);
  if (request == NULL) { 
    terminate("Invalid git request.\n"); 
  }

  file_stat = check_path(request->repo_path);
  if (file_stat != 0) { 
    free(request); 
    return 1; 
  }

  if (strlen(EXEC_COMMAND) == 0) {
    fprintf(stderr, "No replace command defined\n");
    free(request);
    return 1;
  }

  setenv("GITPROXY_USER", app_user, 1);
  setenv("GITPROXY_KEY", app_key, 1);
  setenv("GITPROXY_ROOT", GIT_ROOT, 1);
  setenv("GITPROXY_REPO", request->repo, 1);
  setenv("GITPROXY_ACTION", request->action, 1);
  setenv("GITPROXY_COMMAND", request->command, 1);
  setenv("GITPROXY_IS_READ", request->is_read ? "1" : "0", 1);
  setenv("GITPROXY_IS_WRITE", request->is_write ? "1" : "0", 1);

  free(request);

  int exitcode = execlp(EXEC_COMMAND, EXEC_COMMAND, EXEC_ARG, 0);
  return exitcode;
}