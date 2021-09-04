#pragma once

#include "q_angle.h"
#include "c_user_cmd.h"
#include "i_move_helper.h"

class CMoveData
{
public:
    bool    m_bFirstRunOfFunctions : 1;
    bool    m_bGameCodeMovedPlayer : 1;
    int     m_nPlayerHandle;        // edict index on server, client entity handle on client=
    int     m_nImpulseCommand;      // Impulse command issued.
    Vector  m_vecViewAngles;        // Command view angles (local space)
    Vector  m_vecAbsViewAngles;     // Command view angles (world space)
    int     m_nButtons;             // Attack buttons.
    int     m_nOldButtons;          // From host_client->oldbuttons;
    float   m_flForwardMove;
    float   m_flSideMove;
    float   m_flUpMove;
    float   m_flMaxSpeed;
    float   m_flClientMaxSpeed;
    Vector  m_vecVelocity;          // edict::velocity        // Current movement direction.
    Vector  m_vecAngles;            // edict::angles
    Vector  m_vecOldAngles;
    float   m_outStepHeight;        // how much you climbed this move
    Vector  m_outWishVel;           // This is where you tried 
    Vector  m_outJumpVel;           // This is your jump velocity
    Vector  m_vecConstraintCenter;
    float   m_flConstraintRadius;
    float   m_flConstraintWidth;
    float   m_flConstraintSpeedFactor;
    float   m_flUnknown[5];
    Vector  m_vecAbsOrigin;
};

class c_base_entity;

class IGameMovement
{
public:
    virtual			~IGameMovement(void) {}

    virtual void	          ProcessMovement(c_base_entity* pPlayer, CMoveData* pMove) = 0;
    virtual void	          Reset(void) = 0;
    virtual void	          StartTrackPredictionErrors(c_base_entity* pPlayer) = 0;
    virtual void	          FinishTrackPredictionErrors(c_base_entity* pPlayer) = 0;
    virtual void	          DiffPrint(char const* fmt, ...) = 0;
    virtual Vector const& GetPlayerMins(bool ducked) const = 0;
    virtual Vector const& GetPlayerMaxs(bool ducked) const = 0;
    virtual Vector const& GetPlayerViewOffset(bool ducked) const = 0;
    virtual bool		        IsMovingPlayerStuck(void) const = 0;
    virtual c_base_entity* GetMovingPlayer(void) const = 0;
    virtual void		        UnblockPusher(c_base_entity* pPlayer, c_base_entity* pPusher) = 0;
    virtual void            SetupMovementBounds(CMoveData* pMove) = 0;
};

class CGameMovement
    : public IGameMovement
{
public:
    virtual ~CGameMovement(void) {}
};

class IPrediction
{
public:
    bool InPrediction()
    {
        typedef bool(__thiscall* oInPrediction)(void*);
        return call_virtual<oInPrediction>(this, 14)(this);
    }

    void RunCommand(c_base_entity* player, CUserCmd* ucmd, IMoveHelper* moveHelper)
    {
        typedef void(__thiscall* oRunCommand)(void*, c_base_entity*, CUserCmd*, IMoveHelper*);
        return call_virtual<oRunCommand>(this, 19)(this, player, ucmd, moveHelper);
    }

    void SetupMove(c_base_entity* player, CUserCmd* ucmd, IMoveHelper* moveHelper, void* pMoveData)
    {
        typedef void(__thiscall* oSetupMove)(void*, c_base_entity*, CUserCmd*, IMoveHelper*, void*);
        return call_virtual<oSetupMove>(this, 20)(this, player, ucmd, moveHelper, pMoveData);
    }

    void FinishMove(c_base_entity* player, CUserCmd* ucmd, void* pMoveData)
    {
        typedef void(__thiscall* oFinishMove)(void*, c_base_entity*, CUserCmd*, void*);
        return call_virtual<oFinishMove>(this, 21)(this, player, ucmd, pMoveData);
    }

    void SetLocalAngles(Vector& angle) {
        using original_fn = void(__thiscall*)(void*, Vector&);
        call_virtual< original_fn >(this, 13)(this, angle);
    }
};