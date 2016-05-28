/*
  Patterns Sketch

  This is a sketch for sending USB MIDI clock and triggers
  that are following several algorithms.

  Created 5/24/2016
  By Nikita Griaznov

  https://github.com/ngriaznov/patterns

*/

#include "EuclidianGenerator.h"
#define _TASK_MICRO_RES
#define _TASK_TIMECRITICAL
#define _TASK_PRIORITY
#include <TaskScheduler.h>
#include "Patterns.h"

TriggerGenerator**            generators          = new TriggerGenerator*[quantity];
Scheduler                     scheduler;
Task                          clockTask(clockInterval, -1, &clockCallback, &scheduler);
Task                          notifyTask(clockInterval*24, -1, &notifyOnCallback, &scheduler);

// setup everything
void setup(void) {
  // reserve led action
  pinMode(ledPin, OUTPUT);

  // limit range
  bpm = constrain(bpm, 60.0, 240.0);
  
  // initialize clock interrupt, interval is in microseconds
  clockInterval = 60000000/(24 * bpm);
  
  // give some time for host device to initialize (optional)
  delay(2000);

  // setup generators
  for (int i = 1; i <= quantity; i++) {
    generators[i] = new EuclidianGenerator();
    generators[i]->seed();
  }

  usbMIDI.sendRealTime(START);
  
  clockTask.setInterval(clockInterval);
  notifyTask.setInterval(clockInterval*24);
  
  scheduler.startNow();
  
  clockTask.enable();
  notifyTask.enable();
}

/*
 * Notification action.
 */
void notifyOnCallback() {
  notify(true);
  notifyTask.setCallback(&notifyOffCallback);
  notifyTask.delay(1000);
}

/*
 * Notification action.
 */
void notifyOffCallback() {
  notify(false);
  notifyTask.setCallback(&notifyOnCallback);
}

/*
 * Main clock action.
 */
void clockCallback() {
 
  // increase clock count
  clockCount++;

  // run division processing
  // send clock signal
    usbMIDI.sendRealTime(CLOCK);
    for (int i = 1; i <= quantity; i++) {
      if (clockCount%generators[i]->divider == 0) {
        // process next trig
        if (generators[i]->trig()){
          usbMIDI.sendNoteOn(23 + i, 127, 1);
          usbMIDI.sendNoteOff(23 + i, 127, 1);     
        }
      }
    }

    if (clockCount%1536 == 0){
      clockCount = 0;
    }
}

void loop(void) {
  usbMIDI.read();  
  scheduler.execute();
}
