// Brings the jump0 game window to the foreground.
// Lives in the repo (not /tmp) so no other local process can replace it.
import AppKit

for app in NSWorkspace.shared.runningApplications where app.localizedName == "jump0" {
    app.activate(options: [.activateIgnoringOtherApps])
}
