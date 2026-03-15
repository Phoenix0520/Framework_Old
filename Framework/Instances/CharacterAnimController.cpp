#include "CharacterAnimController.h"

void CharacterAnimController::BuildGraph()
{
	auto idle = AnimGraph::CreateClip("IDLE");
	auto walk = AnimGraph::CreateClip("WALK");
	auto die = AnimGraph::CreateClip("DEATH");
	auto atk = AnimGraph::CreateClip("ATK");
	die->SetLoop(false);
	atk->SetLoop(false);

	//auto locomotion = AnimGraph::CreateBlend1D(idle, walk);

	auto stateMachine = AnimGraph::CreateStateMachine();

	AnimState idleState;
	idleState.node = idle;

	AnimState walkState;
	walkState.node = walk;

	AnimState deadState;
	deadState.node = die;
	
	AnimState atkState;
	atkState.node = atk;

	AnimTransition* t1 = new AnimTransition();
	t1->SetTargetState("WALK");
	t1->SetBlendTime(0.2f);

	AnimCondition* cond1 = new AnimCondition();
	cond1->SetParamName("Moving");
	cond1->SetType(AnimConditionType::BOOL_TRUE);

	t1->AddConditions("Moving", cond1);


	AnimTransition* t0 = new AnimTransition();
	t0->SetTargetState("ATK");
	t0->SetBlendTime(0.2f);

	AnimCondition* cond0 = new AnimCondition();
	cond0->SetParamName("Attacking");
	cond0->SetType(AnimConditionType::BOOL_TRUE);

	t0->AddConditions("Attacking", cond0);


	AnimTransition* t2 = new AnimTransition();
	t2->SetTargetState("IDLE");
	t2->SetBlendTime(0.2f);

	AnimCondition* cond2 = new AnimCondition();
	cond2->SetParamName("Moving");
	cond2->SetType(AnimConditionType::BOOL_FALSE);
	t2->AddConditions("Moving", cond2);

	AnimTransition* t3 = new AnimTransition();
	t3->SetTargetState("DEATH");
	t3->SetBlendTime(0.2f);

	AnimCondition* cond3 = new AnimCondition();
	cond3->SetParamName("Dead");
	cond3->SetType(AnimConditionType::BOOL_TRUE);
	t3->AddConditions("Dead", cond3);

	AnimTransition* t4 = new AnimTransition();
	t4->SetType(TransitionType::ANIM_END);
	AnimCondition* cond4 = new AnimCondition();

	idleState.transitions.push_back(t1);
	idleState.transitions.push_back(t3);
	idleState.transitions.push_back(t0);
	walkState.transitions.push_back(t2);
	atkState.transitions.push_back(t4);
	atkState.transitions.push_back(t1);

	stateMachine->AddState("IDLE", idleState);
	stateMachine->AddState("WALK", walkState);
	stateMachine->AddState("DEATH", deadState);
	stateMachine->AddState("ATK", atkState);
	stateMachine->SetState("IDLE");

	animGraph->SetRootNode(stateMachine);
}
