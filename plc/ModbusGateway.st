FUNCTION_BLOCK FB_ModbusGateway
VAR_INPUT
    StateCode : USINT;
    FaultActive : BOOL;
    GuardClosed : BOOL;
    PressureHealthy : BOOL;
    EmergencyStop : BOOL;
    CommandRegister : UINT;
END_VAR
VAR_OUTPUT
    StateRegister : UINT;
    SafetyRegister : UINT;
    AcceptedCommand : UINT;
END_VAR

StateRegister := UINT(StateCode);
SafetyRegister := 0;
IF GuardClosed THEN SafetyRegister := SafetyRegister OR 1; END_IF;
IF PressureHealthy THEN SafetyRegister := SafetyRegister OR 2; END_IF;
IF EmergencyStop THEN SafetyRegister := SafetyRegister OR 4; END_IF;
IF FaultActive THEN SafetyRegister := SafetyRegister OR 8; END_IF;

IF CommandRegister <= 3 THEN
    AcceptedCommand := CommandRegister;
ELSE
    AcceptedCommand := 0;
END_IF;
END_FUNCTION_BLOCK
