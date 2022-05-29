CManagement*	Get_Management(void)
{
	return CManagement::GetInstance();
}

CInfoSubject* Get_InfoSubject(void)
{
	return CInfoSubject::GetInstance();
}

CLayerMgr*	Get_LayerMgr(void)
{
	return CLayerMgr::GetInstance();
}
CCameraMgr*	Get_CameraMgr(void)
{
	return CCameraMgr::GetInstance();
}
CDivisionMgr*	Get_DivisionMgr(void)
{
	return CDivisionMgr::GetInstance();
}