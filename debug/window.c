#include "config.h"
#include "mutt/mutt.h"
#include "gui/lib.h"

extern struct MuttWindow *RootWindow;

const char *win_size(struct MuttWindow *win)
{
  if (!win)
    return "???";

  switch (win->size)
  {
    case MUTT_WIN_SIZE_FIXED:
      return "FIX";
    case MUTT_WIN_SIZE_MAXIMISE:
      return "MAX";
    case MUTT_WIN_SIZE_MINIMISE:
      return "MIN";
  }

  return "???";
}

const char *win_type(struct MuttWindow *win)
{
  if (!win)
    return "UNKNOWN";

  switch (win->type)
  {
    case WT_ROOT:
      return "root";
    case WT_ALL_DIALOGS:
      return "all-dialogs";
    case WT_DIALOG:
      return "dialog";
    case WT_CONTAINER:
      return "container";
    case WT_HELP_BAR:
      return "help-bar";
    case WT_MESSAGE:
      return "message";
    case WT_INDEX:
      return "index";
    case WT_INDEX_BAR:
      return "index-bar";
    case WT_PAGER:
      return "pager";
    case WT_PAGER_BAR:
      return "pager-bar";
    case WT_SIDEBAR:
      return "sidebar";
  }

  return "UNKNOWN";
}

void win_dump(struct MuttWindow *win, int indent)
{
  bool visible = mutt_window_is_visible(win);

  mutt_debug(LL_DEBUG1, "%*s%s[%d,%d] %s %s %s (%d,%d)%s\n", indent, "",
             visible ? "" : "\033[1;30m", win->state.col_offset, win->state.row_offset,
             win_size(win), win->name ? win->name : "", win_type(win),
             win->state.cols, win->state.rows, visible ? "" : "\033[0m");

  struct MuttWindow *np = NULL;
  TAILQ_FOREACH(np, &win->children, entries)
  {
    win_dump(np, indent + 4);
  }
}

void win_serialise(struct MuttWindow *win, struct Buffer *buf)
{
  if (!mutt_window_is_visible(win))
    return;

  mutt_buffer_add_printf(buf, "<%s {%dx,%dy} [%dC,%dR]", win_size(win),
                         win->state.col_offset, win->state.row_offset,
                         win->state.cols, win->state.rows);
  struct MuttWindow *np = NULL;
  TAILQ_FOREACH(np, &win->children, entries)
  {
    win_serialise(np, buf);
  }
  mutt_buffer_addstr(buf, ">");
}

void debug_win_dump(void)
{
  mutt_debug(LL_DEBUG1, "\n");
  win_dump(RootWindow, 0);
  mutt_debug(LL_DEBUG1, "\n");
  struct Buffer buf = mutt_buffer_make(1024);
  win_serialise(RootWindow, &buf);
  mutt_debug(LL_DEBUG1, "%s\n", mutt_b2s(&buf));
  mutt_buffer_dealloc(&buf);
}
