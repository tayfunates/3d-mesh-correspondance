#ifndef TACORE_TIMER_H
#define TACORE_TIMER_H

// To measure time:
// Timer t;  (SOME PROCESSING);  double seconds = t.seconds();

namespace TACore
{

	/**
	* @brief	A high resolution timer class.
	*/
	class Timer
	{
		double	m_start; ///< Last reset time (or the time of construction) in fractional seconds.

	public:
		/**
		* @brief	Default constructor. Inits the 'start time'.
		*/
		Timer();

		/**
		* @brief	Sets the 'start time' to now().
		*/
		void	reset();

		/**
		* @brief	Returns 'now()' - 'start time'.
		* @return	Fractional seconds between the last 'reset()' and 'now()'.
		*/
		double	seconds() const;

		/**
		* @brief	Gets the current time in high-res fractional seconds.
		* @return	Current time as seconds with fractions. Epoch may be different in different platforms, so do not assume any epoch.
		*/
		static double now();
	};

} // namespace TACore

#endif // Timer
