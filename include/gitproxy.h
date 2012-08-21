#define GITPROXY_DEBUG 1
#define GITPROXY_EXEC ""
#define GITPROXY_EXEC_ARG ""

#define GIT_USER "git"
#define GIT_HOME "/home/git"
#define GIT_ROOT "/home/git/repositories"
#define REGEX_GIT_COMMAND "^(git[-|\\s]upload-pack|git[-|\\s]upload-archive|git[-|\\s]receive-pack) '(.*)'$"

typedef struct {
  char *user;                  /* Connection user */
  char *home;                  /* Home directory of user */
  char *ssh_client;            /* SSH client string */
  char *ssh_connection;        /* SSH connection string */
  char *ssh_command;           /* SSH command string */
} g_session;

typedef struct {
  char *command;               /* Git command */
  char *action;                /* Git command action */
  char *repo;                  /* Repository identifier */
  char *repo_path;             /* Full repository path */
  unsigned short is_read;      /* Command read flag */
  unsigned short is_write;     /* Command write flag */
} g_request;