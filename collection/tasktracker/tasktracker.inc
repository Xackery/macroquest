| To use, create a subdirectory called tasks
#Event Task "[MQ2] task"
#Event TaskUpdate "#*# has been updated."
#Event TaskUpdate "You have been assigned the task #*#"

Sub TaskUpdate
	/call TaskSnapshot
/return

Sub TaskSnapshot
	/declare isWindowOpen bool local
	/varset isWindowOpen ${Window[TaskWnd].Open}
	/if (!${Window[TaskWnd].Open}) {
		/windowstate TaskWnd open
		/delay 5
	}

	/declare taskIndex int local
	/declare taskStep int local
	/declare isSnapshotted bool local

	/for taskIndex 1 to ${Window[TaskWnd].Child[TASK_TaskList].Items}
		/varset isSnapshotted FALSE
		/if (${Window[TaskWnd].Child[TASK_TaskList].List[${taskIndex},3].Length}==0) /continue
		/notify TaskWnd TASK_TaskList listselect ${taskIndex}


		/if (!${Ini["tasks\${Window[TaskWnd].Child[TASK_TaskList].List[${taskIndex},3]}.ini","General","timeleft"].Length}) {
			/ini "tasks\${Window[TaskWnd].Child[TASK_TaskList].List[${taskIndex},3]}.ini" "General" "type" "${Window[TaskWnd].Child[TASK_TaskList].List[${taskIndex},2]}"
			/ini "tasks\${Window[TaskWnd].Child[TASK_TaskList].List[${taskIndex},3]}.ini" "General" "name" "${Window[TaskWnd].Child[TASK_TaskList].List[${taskIndex},3]}"
			/ini "tasks\${Window[TaskWnd].Child[TASK_TaskList].List[${taskIndex},3]}.ini" "General" "timeleft" "${Window[TaskWnd].Child[TASK_TaskList].List[${taskIndex},4]}"
			/ini "tasks\${Window[TaskWnd].Child[TASK_TaskList].List[${taskIndex},3]}.ini" "General" "mainzone" "${Window[TaskWnd].Child[TASK_TaskList].List[${taskIndex},5]}"
			/ini "tasks\${Window[TaskWnd].Child[TASK_TaskList].List[${taskIndex},3]}.ini" "General" "time" "${Time.Year}-${Time.Month}-${Time.Day} ${Time}"
		}

		/for taskStep 1 to ${Window[TaskWnd].Child[TASK_TaskElementList].Items}
			/if (${Ini["tasks\${Window[TaskWnd].Child[TASK_TaskList].List[${taskIndex},3]}.ini","Steps","${taskStep}time"].Length}) /continue
			
			|/echo ${Window[TaskWnd].Child[TASK_TaskList].List[${taskIndex},3]} ${Window[TaskWnd].Child[TASK_TaskElementList].List[${taskStep},2]} /if (${Window[TaskWnd].Child[TASK_TaskElementList].List[${taskStep},2].NotEqual[Done]}  && ${Window[TaskWnd].Child[TASK_TaskElementList].List[${taskStep},2].Find[/]}) {
			|/echo condition ${Window[TaskWnd].Child[TASK_TaskList].List[${taskIndex},3]} ${Window[TaskWnd].Child[TASK_TaskElementList].List[${taskStep},1]} ${Window[TaskWnd].Child[TASK_TaskElementList].List[${taskStep},1]}			
			/if (${Window[TaskWnd].Child[TASK_TaskElementList].List[${taskStep},2].Equal[Done]} || !${Window[TaskWnd].Child[TASK_TaskElementList].List[${taskStep},2].Find[/]}) /continue
			/varset isSnapshotted TRUE
			|/echo snapshotting ${Window[TaskWnd].Child[TASK_TaskList].List[${taskIndex},3]} ${Window[TaskWnd].Child[TASK_TaskElementList].List[${taskStep},1]} ${Window[TaskWnd].Child[TASK_TaskElementList].List[${taskStep},1]}
			/ini "tasks\${Window[TaskWnd].Child[TASK_TaskList].List[${taskIndex},3]}.ini" "Steps" "${taskStep}instruction" "${Window[TaskWnd].Child[TASK_TaskElementList].List[${taskStep},1]}"
			/ini "tasks\${Window[TaskWnd].Child[TASK_TaskList].List[${taskIndex},3]}.ini" "Steps" "${taskStep}steps" "${Window[TaskWnd].Child[TASK_TaskElementList].List[${taskStep},2]}"
			/ini "tasks\${Window[TaskWnd].Child[TASK_TaskList].List[${taskIndex},3]}.ini" "Steps" "${taskStep}zone" "${Window[TaskWnd].Child[TASK_TaskElementList].List[${taskStep},3]}"
			/ini "tasks\${Window[TaskWnd].Child[TASK_TaskList].List[${taskIndex},3]}.ini" "Steps" "${taskStep}body" "${Window[TaskWnd].Child[TASK_Description].Text}"
			/ini "tasks\${Window[TaskWnd].Child[TASK_TaskList].List[${taskIndex},3]}.ini" "Steps" "${taskStep}time" "${Time.Year}-${Time.Month}-${Time.Day} ${Time}"
		/next taskStep
		/if (${isSnapshotted}) /echo Snapshotted task ${Window[TaskWnd].Child[TASK_TaskList].List[${taskIndex},3]}
	/next taskIndex

	/if (!${isWindowOpen}) {
		/keypress alt+q
		/doevents
		/delay 5
	}
	/echo Snapshotted Tasks
/return

Sub Event_TaskUpdate(line, taskName, eventParams)
	/call TaskSnapshot
/return

Sub Event_TaskAssign(line, taskName, eventParams)
	/call TaskSnapshot
/return

Sub Event_Task(line, taskName, eventParams)
	/call TaskSnapshot
/return