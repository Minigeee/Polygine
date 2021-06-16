#ifndef POLY_AUDIO_FILE_WRITER_H
#define POLY_AUDIO_FILE_WRITER_H

#include <poly/Core/Stream.h>

#include <SFML/Audio/OutputSoundFile.hpp>

#include <mutex>

namespace poly
{


///////////////////////////////////////////////////////////
/// \brief A file writer for audio data
///
///////////////////////////////////////////////////////////
class AudioFileWriter : public WriteStream
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default constructor
	///
	///////////////////////////////////////////////////////////
	AudioFileWriter();

	///////////////////////////////////////////////////////////
	/// \brief Open an audio file for writing
	///
	/// This opens a file but does not write any data into it.
	/// The acceptable file formats are: WAV, OGG/Vorbis, and FLAC.
	///
	/// \param fname The path to the file
	/// \param numChannels The number of channels of the saved audio data
	/// \param sampleRate The sample rate of the saved audio data
	///
	/// \return True if the file was successfully opened for writing
	///
	///////////////////////////////////////////////////////////
	bool open(const std::string& fname, Uint32 numChannels, Uint32 sampleRate);

	///////////////////////////////////////////////////////////
	/// \brief Close the file if it is open
	///
	///////////////////////////////////////////////////////////
	void close();

	///////////////////////////////////////////////////////////
	/// \brief Write data into an opened file
	///
	/// The file must be opened for this function to work. Data
	/// written using this function is treated like bytes, which
	/// is half the size of an audio sample.
	///
	/// \param data A pointer to the data to write into the file
	/// \param size The number of bytes to write into the file
	///
	/// \return The number of bytes that was actually written
	///
	///////////////////////////////////////////////////////////
	Uint32 write(void* data, Uint32 size) override;

	///////////////////////////////////////////////////////////
	/// \brief Write data into an opened file
	///
	/// The file must be opened for this function to work. Data
	/// written using this function is treated as samples.
	///
	/// \param data A pointer to the data to write into the file
	/// \param size The number of samples to write into the file
	///
	///////////////////////////////////////////////////////////
	void write(Int16* samples, Uint32 num);

	///////////////////////////////////////////////////////////
	/// \brief Save data from an input read stream into an open file
	///
	/// Data from the input stream is written in chunks of samples
	/// equivalent to the length of 1 second, until there is no
	/// more data in the input stream to read.
	///
	/// \param input A pointer to the input stream
	///
	///////////////////////////////////////////////////////////
	void save(ReadStream* input);

	///////////////////////////////////////////////////////////
	/// \brief Save data from all piped input read streams into an open file
	///
	/// For each piped input read stream connected to the writer,
	/// data is read in chunks of samples equivalent to 1 second of
	/// audio. Chunks are read from each stream until no more data is
	/// available to be read from the streams. Streams are written
	/// in the order they were piped into the file writer.
	///
	///////////////////////////////////////////////////////////
	void save();

	///////////////////////////////////////////////////////////
	/// \brief Check if the writer has opened a file
	///
	/// \return True if the writer has opened a file
	///
	///////////////////////////////////////////////////////////
	bool isOpen() const;

private:
	sf::OutputSoundFile m_file;		//!< The internal SFML output sound file
	Uint32 m_sampleRate;			//!< The sample rate to save data in
	Uint32 m_numChannels;			//!< The number of channels to save data in
	std::mutex m_mutex;				//!< A mutex used to protect write access in multithreaded programs
	bool m_isOpen;					//!< A boolean indicating if the writer has an open file
};


}

#endif


///////////////////////////////////////////////////////////
/// \class poly::AudioFileWriter
/// \ingroup Audio
///
/// The audio file writer is used to save audio data into files
/// of valid formats. The writer is a write stream, so data
/// can be streamed into it like any other write stream, as long
/// as it has opened a valid file. To save data from read streams
/// that don't push their data (Music and AudioBuffer), use the
/// save() function to read and save all data until no more data
/// is available in the read streams.
///
/// Usage example:
/// \code
///
/// using namespace poly;
///
/// // Convert audio formats
/// {
///		Music music;
///		music.open("music.wav");
///
///		AudioFileWriter writer;
///		writer.open("music.flac", music.getNumChannels(), music.getSampleRate());
///		
///		music.pipe(&writer);
///		writer.save();
///		writer.close();
/// }
///
/// // Save recording to file
/// {
///		AudioFileWriter writer;
///		writer.open("recording.flac", 1, 44100);
///
///		AudioRecorder recorder;
///		recorder.pipe(&writer);
///		recorder.start();
///
///		// Wait some time...
///
///		recorder.stop();
///		writer.close();
/// }
///
/// \endcode
///
///////////////////////////////////////////////////////////