#include "EventCreator.h"

EventCreator* EventCreator::m_pInstance = 0;

EventCreator& EventCreator::getInstance()
{
	if (m_pInstance == 0)
	{
		m_pInstance = new EventCreator();
	}
	return *m_pInstance;
}

void EventCreator::bind(NetworkManager* networkmanager, EventQueue* eventqueue, vector<Player*>* players)
{
	m_pNetworkManager = networkmanager;
	m_pEventQueue = eventqueue;
	m_pPlayers = players;
}

bool EventCreator::createBuildEvent(const double time, const unsigned int requestId, const int type, const float angle, Player& currentPlayer, Cell& cell)
{
	const float* const position = currentPlayer.getPopulation().getRelativePosition(cell.getId(), angle);

	for (unsigned int i = 0; i < m_pPlayers->size(); ++i)
	{
		const vector<Cell*>& cells = 
			(*m_pPlayers)[i]->getPopulation().findInRadiusOf(position, cell.getRadius());
		if (cells.size() > 0)
		{
			/// collision detected
			CreateCellFailure failure;
			failure.requestId = requestId;
			failure.errorCode = CreateCellErrorCode::SpotAlreadyTaken;
			m_pNetworkManager->send(failure);
			return false;
		}
	}

	currentPlayer.getPopulation().createCell(&cell, position, angle);

	if (cell.getId() < 0)
	{
		/**
			* TODO: add enum for creation errors
			* NOW:  -1 for all creation errors
			*/
		return false;
	}

	BuildingEvent* be = new BuildingEvent(time, *m_pNetworkManager, cell, *m_pPlayers);
	m_pEventQueue->addEvent(be);
		
	CreateCellSuccess success;
	success.requestId = requestId;
	success.cellId = cell.getId();
	success.position[0] = cell.getPosition()[0];
	success.position[1] = cell.getPosition()[1];
	success.angle = cell.getAngle();
	m_pNetworkManager->send(success);

	for (unsigned int i = 0; i < m_pPlayers->size(); ++i)
	{
		if ((*m_pPlayers)[i]->getId() != currentPlayer.getId())
		{
			CellNew cellnew;
			cellnew.playerId = currentPlayer.getId();
			cellnew.cellId = cell.getId();
			cellnew.position[0] = cell.getPosition()[0];
			cellnew.position[1] = cell.getPosition()[1];
			cellnew.type = type;
			cellnew.endpoint = (*m_pPlayers)[i]->getEndpoint();
		}
	}

	delete position;

	return true;
}

bool EventCreator::createAttackEvent(const double time, const bool isAttacker, const Player& currentPlayer, Cell& currentCell)
{
	if (!(m_pNetworkManager && m_pEventQueue && m_pPlayers))
	{
		/// call bind first
		return false;
	}

	if (isAttacker && (!currentCell.isComplete() || dynamic_cast<StandardCell*>(&currentCell) == 0))
	{
		/// cell could not be an attacker
		return false;
	}

	for (unsigned int i = 0; i < m_pPlayers->size(); ++i)
	{
		if ((*m_pPlayers)[i]->getId() != currentPlayer.getId())
		{
			/** 
				* search for all cells that would be in its attack radius
				* reversely all the other cells that are attackers have the current cell in its attack radius
				*/
			const vector<Cell*>& cells =
				(*m_pPlayers)[i]->getPopulation().findInRadiusOf(currentCell.getPosition(), currentCell.getRadius() + StandardCell::m_fAttackRadius);
					
			ci::Vec2f attacker;
			ci::Vec2f victim;
			/// those assignments are true for all iterations
			if (isAttacker)
			{
				attacker = ci::Vec2f(currentCell.getPosition());
			}
			else
			{
				ci::Vec2f victim(currentCell.getPosition());
			}
			for (vector<Cell*>::const_iterator it = cells.begin(); it != cells.end(); ++it)
			{
				if ((*it)->isComplete())
				{
					/// those assignments depend on the iterator
					if (isAttacker)
					{
						victim = ci::Vec2f((*it)->getPosition());
					}
					else
					{
						if ((*it)->isComplete() && dynamic_cast<StandardCell*>(*it) == 0) continue;
						ci::Vec2f attacker((*it)->getPosition());
					}

					ci::Vec2f attackerDir;
					attackerDir.rotate((*it)->getAngle() * (float)M_PI / 180.f);
					attackerDir.normalize();

					ci::Vec2f attacker2victim = victim - attacker;
					attacker2victim.normalize();

					float dot = abs(attackerDir.dot(attacker2victim));

					/// damage modifier 10.f
					float damage = 1.f - dot * 10.f;
					damage = damage < 0.2f ? 0.2f : damage;

					AttackEvent* ae = new AttackEvent(time, *m_pNetworkManager, *m_pEventQueue, *(*it), currentCell, damage, *m_pPlayers);
					m_pEventQueue->addEvent(ae);

					/// attack message is sent in event
				}
			}
		}
	}
	return true;
}

EventCreator::EventCreator()
{ }