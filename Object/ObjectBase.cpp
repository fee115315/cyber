#include "ObjectBase.h"

ObjectBase::ObjectBase()
{
	m_pMain = nullptr;
	m_isExist = false;
}

bool ObjectBase::isCol(ObjectBase& target)
{
	// ‘¶İ‚µ‚È‚¢•¨‘Ì‚Í“–‚½‚ç‚È‚¢
	if (!m_isExist)	return false;
	if (!target.isExist())	return false;
	// None‚Í‰½‚É‚à‚Ô‚Â‚©‚ç‚È‚¢
	if (getColType() == ColType::kNone)	return false;
	if(getTargetColType() == ColType::kNone)	return false;
	// ©g‚ª‚Ô‚Â‚©‚é‘ÎÛˆÈŠO‚É‚Í‚Ô‚Â‚©‚ç‚È‚¢
	if (getTargetColType() != target.getColType())	return false;

	if (target.getRight() < getLeft())	return false;
	if (target.getLeft() > getRight())	return false;
	if (target.getTop() > getBottom())	return false;
	if (target.getBottom() < getTop())	return false;
	return true;
}