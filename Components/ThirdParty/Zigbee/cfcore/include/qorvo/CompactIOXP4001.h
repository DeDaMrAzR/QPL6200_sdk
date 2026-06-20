// CompactIOXP4001.h : header file
//
// Copyright(C) 2023 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// Provides a minimal stub placeholder for I/O support on the  QPG6200 (XP4001).


/////////////////////////////////////////////////////////////////////////////
// forward references classes

class CCompactIRQ;


/////////////////////////////////////////////////////////////////////////////
// CCompactIO

class CCompactIO
{
	// Attributes
	public:
		// Returns true, if the IO is actually used on the board, or is left
		// unconnected
		bool IsConnected() const;

		// Returns true, if the IO is currently configured as an input
		bool IsInput() const;

		// Returns true, if the IO is currently configured as an output
		bool IsOutput() const;

		// Returns true, if the IO is currently configured for peripheral A
		bool IsPeripheral() const;

		// Returns the current logical level on the package pin
		bool GetInputState() const;

		// Returns the current logical level driven by the output pin
		bool GetOutputState() const;

	// Operations
	public:
		void Configure() const;

		// Sets the logical level driven by the package pin when configured
		// as output to either low or high
		void SetOutputState(const bool high = true) const;

		// Sets the logical level driven by the package pin when configured
		// as output to high
		void SetOutput() const;

		// Sets the logical level driven by the package pin when configured
		// as output to low
		void ClearOutput() const;

		// Toggles the output pin
		void Toggle() const;
};


inline bool CCompactIO::IsConnected() const
{
	return false;
}


inline void CCompactIO::SetOutput() const
{
}


inline void CCompactIO::ClearOutput() const
{
}


inline void CCompactIO::Configure() const
{

}


inline void CCompactIO::SetOutputState(const bool bHigh) const
{
}


inline bool CCompactIO::IsInput() const
{
	return false;
}


inline bool CCompactIO::IsOutput() const
{
	return false;
}


inline bool CCompactIO::IsPeripheral() const
{
	return false;
}


inline bool CCompactIO::GetOutputState() const
{
	return  false;
}


inline void CCompactIO::Toggle() const
{
}


inline bool CCompactIO::GetInputState() const
{
	return false;
}

