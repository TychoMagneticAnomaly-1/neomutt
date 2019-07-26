/**
 * @file
 * Convenience wrapper for the debug headers
 *
 * @authors
 * Copyright (C) 2019 Richard Russon <rich@flatcap.org>
 *
 * @copyright
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MUTT_DEBUG_LIB_H
#define MUTT_DEBUG_LIB_H

#include "config.h"
#include "mutt/mutt.h"
#include "core/lib.h"

struct MuttWindow;

// Notify
int         debug_notify_observer(struct NotifyCallback *nc);
const char *get_config_type      (int id);
const char *get_context          (int id);
const char *get_event_type       (enum NotifyType type);
const char *get_global_event     (int id);
const char *get_mailbox_event    (int id);
const char *get_mailbox_type     (enum MailboxType type);
void        notify_dump_account  (struct NotifyCallback *nc);
void        notify_dump_color    (struct NotifyCallback *nc);
void        notify_dump_command  (struct NotifyCallback *nc);
void        notify_dump_config   (struct NotifyCallback *nc);
void        notify_dump_context  (struct NotifyCallback *nc);
void        notify_dump_email    (struct NotifyCallback *nc);
void        notify_dump_global   (struct NotifyCallback *nc);
void        notify_dump_mailbox  (struct NotifyCallback *nc);

// Parse Set
void test_parse_set(void);

// Window
void        debug_win_dump(void);
void        win_dump      (struct MuttWindow *win, int indent);
const char *win_size      (struct MuttWindow *win);
const char *win_type      (struct MuttWindow *win);

#endif /* MUTT_DEBUG_LIB_H */
