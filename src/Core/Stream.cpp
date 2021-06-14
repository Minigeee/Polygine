#include <poly/Core/Allocate.h>
#include <poly/Core/Stream.h>

namespace poly
{


///////////////////////////////////////////////////////////
ReadStream::ReadStream()
{

}


///////////////////////////////////////////////////////////
ReadStream::~ReadStream()
{

}


///////////////////////////////////////////////////////////
ReadStream::ReadStream(ReadStream&& other)
{
	m_outputs = std::move(other.m_outputs);

	// Update all pipe bindings
	for (Uint32 i = 0; i < m_outputs.size(); ++i)
	{
		WriteStream* output = m_outputs[i];
		for (Uint32 j = 0; j < output->m_inputs.size(); ++j)
		{
			if (output->m_inputs[j] == &other)
				output->m_inputs[j] = this;
		}
	}
}


///////////////////////////////////////////////////////////
ReadStream& ReadStream::operator=(ReadStream&& other)
{
	if (&other != this)
	{
		m_outputs = std::move(other.m_outputs);

		// Update all pipe bindings
		for (Uint32 i = 0; i < m_outputs.size(); ++i)
		{
			WriteStream* output = m_outputs[i];
			for (Uint32 j = 0; j < output->m_inputs.size(); ++j)
			{
				if (output->m_inputs[j] == &other)
					output->m_inputs[j] = this;
			}
		}
	}

	return *this;
}


///////////////////////////////////////////////////////////
void ReadStream::pipe(WriteStream* output)
{
	m_outputs.push_back(output);
	output->m_inputs.push_back(this);
}


///////////////////////////////////////////////////////////
void ReadStream::unpipe(WriteStream* output)
{
	std::vector<ReadStream*> inputs = output->m_inputs;
	for (auto it = inputs.begin(); it != inputs.end(); ++it)
	{
		if (*it == this)
		{
			inputs.erase(it);
			break;
		}
	}

	for (auto it = m_outputs.begin(); it != m_outputs.end(); ++it)
	{
		if (*it == output)
		{
			m_outputs.erase(it);
			break;
		}
	}
}


///////////////////////////////////////////////////////////
WriteStream::WriteStream()
{

}


///////////////////////////////////////////////////////////
WriteStream::~WriteStream()
{

}


///////////////////////////////////////////////////////////
WriteStream::WriteStream(WriteStream&& other)
{
	m_inputs = std::move(other.m_inputs);

	// Update all pipe bindings
	for (Uint32 i = 0; i < m_inputs.size(); ++i)
	{
		ReadStream* input = m_inputs[i];
		for (Uint32 j = 0; j < input->m_outputs.size(); ++j)
		{
			if (input->m_outputs[j] == &other)
				input->m_outputs[j] = this;
		}
	}
}


///////////////////////////////////////////////////////////
WriteStream& WriteStream::operator=(WriteStream&& other)
{
	if (&other != this)
	{
		m_inputs = std::move(other.m_inputs);

		// Update all pipe bindings
		for (Uint32 i = 0; i < m_inputs.size(); ++i)
		{
			ReadStream* input = m_inputs[i];
			for (Uint32 j = 0; j < input->m_outputs.size(); ++j)
			{
				if (input->m_outputs[j] == &other)
					input->m_outputs[j] = this;
			}
		}
	}

	return *this;
}


}