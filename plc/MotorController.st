TYPE E_MotorState :
(
    stopped,
    starting,
    running,
    stopping,
    tripped
);
END_TYPE

FUNCTION_BLOCK FB_MotorController
VAR_INPUT
    StartCommand : BOOL;
    StopCommand : BOOL;
    ResetCommand : BOOL;
    EmergencyStop : BOOL;
    GuardClosed : BOOL;
    PressureHealthy : BOOL;
    Overload : BOOL;
    NowMs : UDINT;
    StartDelayMs : UDINT := 1500;
    StopDelayMs : UDINT := 800;
END_VAR
VAR_OUTPUT
    State : E_MotorState := stopped;
    RunOutput : BOOL;
    FaultActive : BOOL;
    TransitionSequence : UDINT;
END_VAR
VAR
    StateSinceMs : UDINT;
    Unsafe : BOOL;
END_VAR

Unsafe := EmergencyStop OR NOT GuardClosed OR NOT PressureHealthy OR Overload;

IF Unsafe THEN
    IF State <> tripped THEN
        State := tripped;
        StateSinceMs := NowMs;
        TransitionSequence := TransitionSequence + 1;
    END_IF;
ELSIF State = tripped THEN
    IF ResetCommand THEN
        State := stopped;
        StateSinceMs := NowMs;
        TransitionSequence := TransitionSequence + 1;
    END_IF;
ELSIF State = stopped AND StartCommand THEN
    State := starting;
    StateSinceMs := NowMs;
    TransitionSequence := TransitionSequence + 1;
ELSIF State = starting AND StopCommand THEN
    State := stopping;
    StateSinceMs := NowMs;
    TransitionSequence := TransitionSequence + 1;
ELSIF State = starting AND (NowMs - StateSinceMs) >= StartDelayMs THEN
    State := running;
    StateSinceMs := NowMs;
    TransitionSequence := TransitionSequence + 1;
ELSIF State = running AND StopCommand THEN
    State := stopping;
    StateSinceMs := NowMs;
    TransitionSequence := TransitionSequence + 1;
ELSIF State = stopping AND (NowMs - StateSinceMs) >= StopDelayMs THEN
    State := stopped;
    StateSinceMs := NowMs;
    TransitionSequence := TransitionSequence + 1;
END_IF;

FaultActive := State = tripped;
RunOutput := State = running;
END_FUNCTION_BLOCK
