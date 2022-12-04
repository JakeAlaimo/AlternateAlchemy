Scriptname AlternateAlchemySyringeHit extends ActiveMagicEffect

AlternateAlchemyPoisonMonitor Property PoisonMonitor auto

Event OnEffectStart(Actor Target, Actor Caster)

    bool PreHitLeftHandBeneficialPoison = PoisonMonitor.LeftHandBeneficialPoison
    bool PreHitRightHandBeneficialPoison = PoisonMonitor.RightHandBeneficialPoison

    PoisonMonitor.UpdatePoisonStatus()

    bool PostHitLeftHandBeneficialPoison = PoisonMonitor.LeftHandBeneficialPoison
    bool PostHitRightHandBeneficialPoison = PoisonMonitor.RightHandBeneficialPoison

    bool BeneficialPoisonApplied = (PreHitLeftHandBeneficialPoison && !PostHitLeftHandBeneficialPoison) || (PreHitRightHandBeneficialPoison && !PostHitRightHandBeneficialPoison)

    If (BeneficialPoisonApplied || Target.IsPlayerTeammate())
        return
    EndIf

    If (PoisonMonitor.GetFriendHits() < 3 && Target.GetRelationshipRank(Caster) > 1)
        PoisonMonitor.IncrementFriendHits()
        return
    EndIf

    Target.SendAssaultAlarm()
EndEvent