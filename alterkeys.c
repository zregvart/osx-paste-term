#include <ApplicationServices/ApplicationServices.h>
#include <Carbon/Carbon.h>
#include <libproc.h>
#include <stdio.h>

CGEventRef trapPaste(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
    if (type != kCGEventKeyDown)
        return event;

    CGKeyCode keycode = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);

    CGEventFlags flags = CGEventGetFlags(event);

    if (keycode == kVK_ANSI_V && (flags & kCGEventFlagMaskCommand) == kCGEventFlagMaskCommand) {
        int pid = (int) CGEventGetIntegerValueField(event, kCGEventTargetUnixProcessID);
        char pathbuf[PROC_PIDPATHINFO_MAXSIZE];

        if (!proc_pidpath(pid, pathbuf, sizeof(pathbuf)))
            return event;

        if (strstr(pathbuf, "/Terminal.app/")) {
            FILE *in = popen("/usr/bin/pbpaste -Prefer txt", "r"), *out = popen("/usr/bin/pbcopy", "w");
            char buffy[64];
            memset(buffy, 0, sizeof(buffy) / sizeof(char));

            while (!feof(in) && fgets(buffy, sizeof(buffy) / sizeof(char), in)) {
                for (int i = 0; i < 64 && buffy[i] != 0; i++) {
                    buffy[i] = isprint(buffy[i]) ? buffy[i] : '_';
                }
                fputs(buffy, out);
            }
            pclose(in);
            pclose(out);
        }
    }

    return event;
}

int main(void) {
    CFMachPortRef      eventTap;
    CGEventMask        eventMask;
    CFRunLoopSourceRef runLoopSource;

    eventMask = ((1 << kCGEventKeyDown) | (1 << kCGEventKeyUp));
    eventTap = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, 0, eventMask, trapPaste, NULL);
    if (!eventTap) {
        fprintf(stderr, "failed to create event tap\n");
        exit(1);
    }

    runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);

    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);

    CGEventTapEnable(eventTap, true);

    CFRunLoopRun();
}

