# TaskTracker
This tool snapshots tasks when an update happens, collecting data to make task creation into eqemu easier

# How to use
In your macroquest main script, that ideally loops, you simply need to add the line `#include tasktracker.inc`
You can trigger it manually by using `/echo task`
Otherwise, when assigned a task or a task update occurs, it will open up task window and iterate through the list.
*Note:* It will pop up the Task Window for scanning. If this is annoying, I suggest keeping the task window minimized so it isn't as spammy.