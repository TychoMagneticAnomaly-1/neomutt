#ifndef MUTT_COMMAND_PARSE_H
#define MUTT_COMMAND_PARSE_H

#include "mutt_commands.h"

struct Buffer;

/* functions used to parse commands in a rc file */
enum CommandResult parse_alias           (struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);
enum CommandResult parse_alternates      (struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);
enum CommandResult parse_attachments     (struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);
enum CommandResult parse_echo            (struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);
enum CommandResult parse_finish          (struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);
enum CommandResult parse_group           (struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);
enum CommandResult parse_ifdef           (struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);
enum CommandResult parse_ignore          (struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);
enum CommandResult parse_lists           (struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);
enum CommandResult parse_mailboxes       (struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);
enum CommandResult parse_my_hdr          (struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);
#ifdef USE_SIDEBAR
enum CommandResult parse_path_list       (struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);
enum CommandResult parse_path_unlist     (struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);
#endif
enum CommandResult parse_set             (struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);
enum CommandResult parse_setenv          (struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);
enum CommandResult parse_source          (struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);
enum CommandResult parse_spam_list       (struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);
enum CommandResult parse_stailq          (struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);
enum CommandResult parse_subjectrx_list  (struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);
enum CommandResult parse_subscribe       (struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);
#ifdef USE_IMAP
enum CommandResult parse_subscribe_to    (struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);
#endif
enum CommandResult parse_tag_formats     (struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);
enum CommandResult parse_tag_transforms  (struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);
enum CommandResult parse_unalias         (struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);
enum CommandResult parse_unalternates    (struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);
enum CommandResult parse_unattachments   (struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);
enum CommandResult parse_unignore        (struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);
enum CommandResult parse_unlists         (struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);
enum CommandResult parse_unmailboxes     (struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);
enum CommandResult parse_unmy_hdr        (struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);
enum CommandResult parse_unstailq        (struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);
enum CommandResult parse_unsubjectrx_list(struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);
enum CommandResult parse_unsubscribe     (struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);
#ifdef USE_IMAP
enum CommandResult parse_unsubscribe_from(struct Buffer *buf, struct Buffer *s, unsigned long data, struct Buffer *err);
#endif

void clear_source_stack(void);
int source_rc(const char *rcfile_path, struct Buffer *err);

#endif /* MUTT_COMMAND_PARSE_H */
