#pragma once

class Cell
{
private:
	unsigned int m_id;
	float m_position[3];

	void setPosition(const float position[3])
	{
		m_position[0] = position[0];
		m_position[1] = position[1];
		m_position[2] = position[2];
	}
public:
	Cell(unsigned int m_id, const float position[3])
	{
		setPosition(position);
	}

	Cell(const Cell& cpy)
	{
		m_id = cpy.m_id;
		setPosition(cpy.m_position);
	}

	unsigned int getId() const { return m_id; }
	const float* const getPosition() const
	{
		return m_position;
	}
};