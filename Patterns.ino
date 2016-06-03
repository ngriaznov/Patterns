/*
  Patterns Sketch

  This is a sketch for sending USB MIDI clock and triggers
  that are following several algorithms.

  Created 5/24/2016
  By Nikita Griaznov

  https://github.com/ngriaznov/patterns

*/

#define _TASK_MICRO_RES
#define _TASK_TIMECRITICAL
#define _TASK_PRIORITY
#define _TASK_LTS_POINTER
#define _TASK_WDT_IDS
#include <TaskScheduler.h>
#include "Patterns.h"
#include "EuclidianGenerator.h"

TriggerGenerator** generators = new TriggerGenerator*[quantity];
Task** generatorTasks = new Task*[quantity];
Scheduler scheduler;
Task clockTask(clockInterval, -1, &clockCallback, &scheduler);
Task notifyTask(clockInterval * 24, -1, &notifyOnCallback, &scheduler);

void setup(void) {
  // reserve led action
  pinMode(ledPin, OUTPUT);

  // limit range
  bpm = constrain(bpm, 60.0, 240.0);

  // initialize clock interrupt, interval is in microseconds
  clockInterval = 60000000 / (24 * bpm);

  // give some time for host device to initialize (optional)
  delay(2000);

  // start sequencing
  resetGenerators();
  setupSequence();

  // start scheduler
  scheduler.startNow();
}

/*
   Reset generators.
*/
void resetGenerators() {
  // setup generators
  for (int i = 1; i <= quantity; i++) {
    // use little shift for the notes to shift to C2
    generators[i] = new EuclidianGenerator(i + 23, i);
    generators[i]->seed();
  }
}

/*
 * Restart tasks routine. May be needed when everything is out if sync.
 */
void restartTasks() {
  clockTask.restart();
  notifyTask.restart();

  for (int i = 1; i <= quantity; i++) {
    generatorTasks[i]->restart();
  }
}

/*
   Setup sequence.
*/
void setupSequence() {
  for (int i = 1; i <= quantity; i++) {
    // pass generator to the task and let the scheduler do the scheduling
    // interval should be equal to divider multiplied by clock interval
    generatorTasks[i] = new Task(clockInterval * generators[i]->divider, -1,
                                 &trigCallback, &scheduler);
    generatorTasks[i]->setInterval(clockInterval * generators[i]->divider);
    generatorTasks[i]->enable();
    generatorTasks[i]->setLtsPointer(generators[i]);
  }

  // set intervals
  clockTask.setInterval(clockInterval);
  notifyTask.setInterval(clockInterval * 24);

  // enable main tasks
  clockTask.enable();
  notifyTask.enable();
}

/*
   Notification action.
*/
void notifyOnCallback() {
  notify(true);
  notifyTask.setCallback(&notifyOffCallback);
  notifyTask.delay(1000);
}

/*
   Notification action.
*/
void notifyOffCallback() {
  notify(false);
  notifyTask.setCallback(&notifyOnCallback);
}

/*
   Main clock action.
*/
void clockCallback() {
  // increase clock count
  clockCount++;

  if (!started) {
    started = true;
    usbMIDI.sendRealTime(START);
  }
  // send clock signal
  usbMIDI.sendRealTime(CLOCK);

  if (clockCount % 6144 == 0) {
    clockCount = 0;
    resetGenerators();
  }
}

/*
   Triggel callback routine.
*/
void trigCallback() {
  Task& task = scheduler.currentTask();
  TriggerGenerator& var = *((TriggerGenerator*)task.getLtsPointer());
  var.trig();
}

void loop(void) {
  usbMIDI.read();
  scheduler.execute();
}
