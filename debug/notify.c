#include "config.h"
#include "mutt/mutt.h"
#include "config/lib.h"
#include "email/lib.h"
#include "core/lib.h"
#include "gui/lib.h"
#include "context.h"
#include "globals.h"

extern const struct Mapping Fields[];
extern const struct Mapping ComposeFields[];

const char *get_event_type(enum NotifyType type)
{
  switch (type)
  {
    case NT_ACCOUNT:
      return "account";
    case NT_COLOR:
      return "color";
    case NT_COMMAND:
      return "command";
    case NT_CONFIG:
      return "config";
    case NT_CONTEXT:
      return "context";
    case NT_EMAIL:
      return "email";
    case NT_GLOBAL:
      return "global";
    case NT_MAILBOX:
      return "mailbox";
    default:
      return "UNKNOWN";
  }
}

const char *get_mailbox_type(enum MailboxType type)
{
  switch (type)
  {
    case MUTT_COMPRESSED:
      return "compressed";
    case MUTT_IMAP:
      return "imap";
    case MUTT_MAILDIR:
      return "maildir";
    case MUTT_MBOX:
      return "mbox";
    case MUTT_MH:
      return "mh";
    case MUTT_MMDF:
      return "mmdf";
    case MUTT_NNTP:
      return "nntp";
    case MUTT_NOTMUCH:
      return "notmuch";
    case MUTT_POP:
      return "pop";
    default:
      return "UNKNOWN";
  }
}

const char *get_global_event(int id)
{
  switch (id)
  {
    case NT_GLOBAL_SHUTDOWN:
      return "shutdown";
    case NT_GLOBAL_STARTUP:
      return "startup";
    case NT_GLOBAL_TIMEOUT:
      return "timeout";
    default:
      return "UNKNOWN";
  }
}

const char *get_config_type(int id)
{
  switch (id)
  {
    case NT_CONFIG_SET:
      return "set";
    case NT_CONFIG_RESET:
      return "reset";
    case NT_CONFIG_INITIAL_SET:
      return "initial_set";
    default:
      return "UNKNOWN";
  }
}

const char *get_mailbox_event(int id)
{
  switch (id)
  {
    case NT_MAILBOX_ADD:
      return "add";
    case NT_MAILBOX_REMOVE:
      return "remove";
    case NT_MAILBOX_CLOSED:
      return "closed";
    case NT_MAILBOX_INVALID:
      return "invalid";
    case NT_MAILBOX_RESORT:
      return "resort";
    case NT_MAILBOX_UPDATE:
      return "update";
    case NT_MAILBOX_UNTAG:
      return "untag";
    default:
      return "UNKNOWN";
  }
}

const char *get_context(int id)
{
  switch (id)
  {
    case NT_CONTEXT_CLOSE:
      return "close";
    case NT_CONTEXT_OPEN:
      return "open";
    default:
      return "UNKNOWN";
  }
}

void notify_dump_account(struct NotifyCallback *nc)
{
  struct EventAccount *ev_a = nc->event_data;
  struct Account *a = ev_a->account;

  mutt_debug(LL_DEBUG1, "\tAccount: %p (%s) %s\n", a,
             get_mailbox_type(a->magic), NONULL(a->name));
}

void notify_dump_color(struct NotifyCallback *nc)
{
  struct EventColor *ev_c = nc->event_data;

  const char *color = NULL;
  const char *scope = "";

  if (nc->event_subtype == MT_COLOR_MAX)
    color = "ALL";

  if (!color)
    color = mutt_map_get_name(nc->event_subtype, Fields);

  if (!color)
  {
    color = mutt_map_get_name(nc->event_subtype, ComposeFields);
    scope = "compose ";
  }

  if (!color)
    color = "UNKNOWN";

  mutt_debug(LL_DEBUG1, "\tColor: %s %s%s (%d)\n", ev_c->set ? "set" : "reset",
             scope, color, nc->event_subtype);
}

void notify_dump_command(struct NotifyCallback *nc)
{
  struct Command *cmd = nc->event_data;

  if (cmd->data < 4096)
    mutt_debug(LL_DEBUG1, "\tCommand: %s, data: %d\n", cmd->name, cmd->data);
  else
    mutt_debug(LL_DEBUG1, "\tCommand: %s, data: %p\n", cmd->name, (void *) cmd->data);
}

void notify_dump_config(struct NotifyCallback *nc)
{
  struct EventConfig *ev_c = nc->event_data;

  struct Buffer value = mutt_buffer_make(128);
  cs_he_string_get(ev_c->cs, ev_c->he, &value);
  mutt_debug(LL_DEBUG1, "\tConfig: %s %s = %s\n",
             get_config_type(nc->event_subtype), ev_c->name, mutt_b2s(&value));
  mutt_buffer_dealloc(&value);
}

void notify_dump_context(struct NotifyCallback *nc)
{
  struct EventContext *ev_c = nc->event_data;

  const char *path = "NONE";
  if (ev_c->context && ev_c->context->mailbox)
    path = mailbox_path(ev_c->context->mailbox);

  mutt_debug(LL_DEBUG1, "\tContext: %s %s\n", get_context(nc->event_subtype), path);
}

void notify_dump_email(struct NotifyCallback *nc)
{
  struct EventEmail *ev_e = nc->event_data;

  mutt_debug(LL_DEBUG1, "\tEmail: %d\n", ev_e->num_emails);
  for (size_t i = 0; i < ev_e->num_emails; i++)
  {
    mutt_debug(LL_DEBUG1, "\t\t: %p\n", ev_e->emails[i]);
  }
}

void notify_dump_global(struct NotifyCallback *nc)
{
  mutt_debug(LL_DEBUG1, "\tGlobal: %s\n", get_global_event(nc->event_subtype));
}

void notify_dump_mailbox(struct NotifyCallback *nc)
{
  struct EventMailbox *ev_m = nc->event_data;

  struct Mailbox *m = ev_m->mailbox;
  const char *path = m ? mailbox_path(m) : "";
  mutt_debug(LL_DEBUG1, "\tMailbox: %s %s\n", get_mailbox_event(nc->event_subtype), path);
}

int debug_notify_observer(struct NotifyCallback *nc)
{
  mutt_debug(LL_DEBUG1, "\033[1;31mNotification:\033[0m %s\n", get_event_type(nc->event_type));

  switch (nc->event_type)
  {
    case NT_ACCOUNT:
      notify_dump_account(nc);
      break;
    case NT_COLOR:
      notify_dump_color(nc);
      break;
    case NT_COMMAND:
      notify_dump_command(nc);
      break;
    case NT_CONFIG:
      notify_dump_config(nc);
      break;
    case NT_CONTEXT:
      notify_dump_context(nc);
      break;
    case NT_EMAIL:
      notify_dump_email(nc);
      break;
    case NT_GLOBAL:
      notify_dump_global(nc);
      break;
    case NT_MAILBOX:
      notify_dump_mailbox(nc);
      break;
    default:
      mutt_debug(LL_DEBUG1, "\tEvent Type: %s\n", nc->event_type);
      mutt_debug(LL_DEBUG1, "\tEvent Sub-type: %d\n", nc->event_subtype);
      mutt_debug(LL_DEBUG1, "\tEvent Data: %p\n", nc->event_data);
      break;
  }

  mutt_debug(LL_DEBUG1, "\tGlobal Data: %p\n", nc->global_data);

  return 0;
}
