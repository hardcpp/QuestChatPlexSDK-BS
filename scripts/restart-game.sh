#!/usr/bin/env bash

set -euo pipefail

package="com.beatgames.beatsaber"
activity="$package/com.unity3d.player.UnityPlayerActivity"

adb shell am force-stop "$package"
adb shell am start "$activity"
sleep 1
adb shell am start "$activity"
