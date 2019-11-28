#include <memory>
#include <fstream>
#include <iostream>
#include <exception>
#include <iostream>

void read_header(std::ifstream& file, uint32_t& channelmask,
		uint32_t& event_size);
int count_events(std::ifstream& file, const uint8_t n_channels,
		const uint16_t bins_per_channel);
size_t convert_single_event(std::ifstream& in_file, std::ofstream& out_file,
		const uint32_t word_count_per_channel, const uint16_t n_channels);
std::streampos write_drifttube_header(std::ofstream& out_file,
		const uint32_t n_channels, const uint32_t n_events,
		const uint32_t event_size);

bool next_event_complete(std::ifstream& in_file, const uint32_t n_channels,
		const uint32_t bytes_per_channel);

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		return -1;
	}
	char* infile_name = argv[1];
	char* outfile_name = argv[2];

	std::ifstream in_file(infile_name, std::ios::binary | std::ios::in);
	std::ofstream out_file(outfile_name, std::ios::binary | std::ios::out);

	uint32_t channelmask = 0;
	uint32_t event_size = 0;
	read_header(in_file, channelmask, event_size);
	uint16_t n_channels = 0;
	for (int i = 0; i < 8; ++i)
	{
		n_channels += (channelmask >> i) & 0b1;
	}
	std::cout << std::hex << "0x" << channelmask << "\t" << event_size << "\t"
			<< n_channels << std::dec << std::endl;
	//eventsize - 4 for subtracting header size from eventsize
	uint16_t words_per_event = (event_size - 4) / n_channels;

	in_file.clear();
	in_file.seekg(0, std::ios::beg);
	std::cout << "Counting events: " << words_per_event * 2 << " bins per channel" << std::endl;
	uint32_t n_events = count_events(in_file, n_channels, words_per_event * 2);
	std::cout << "Counted: " << n_events << " events" << std::endl;
	in_file.clear();
	in_file.seekg(0, std::ios::beg);

	write_drifttube_header(out_file, n_channels, n_events, words_per_event * 2);

	uint32_t events_read = 0;
	while (!in_file.eof() && events_read < n_events)
	{
		try
		{
			uint32_t channelmask = 0;
			uint32_t event_size = 0;
			std::cout << "Event " << events_read << ": Reading header." << std::endl;
			read_header(in_file, channelmask, event_size);
			uint16_t n_channels = 0;
			for (int i = 0; i < 8; ++i)
			{
				n_channels += (channelmask >> i) & 0b1;
			}
			//eventsize - 4 for subtracting header size from eventsize
			std::cout << "Read " << n_channels << " channels." << std::endl;
			uint16_t words_per_event = (event_size - 4) / n_channels;
			uint16_t n_samples_per_event = words_per_event * 2;

			if (next_event_complete(in_file, n_channels, words_per_event * sizeof(uint32_t)))
			{
				convert_single_event(in_file, out_file, words_per_event,
						n_channels);
			}
			else
			{
				std::cout << "Event " << events_read << " not complete" << std::endl;
			}
		} catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}


		++events_read;
	}

	in_file.close();
	out_file.close();

	return 0;
}

int count_events(std::ifstream& file, const uint8_t n_channels,
		const uint16_t bins_per_channel)
{
	uint64_t n_bytes_of_data = 0;

	while (!file.eof())
	{
		uint32_t word = 0;
		file.read((char*) &word, sizeof(uint32_t));

		n_bytes_of_data += sizeof(uint32_t);
	}


	return n_bytes_of_data / ((n_channels * bins_per_channel * sizeof(uint16_t)) + 16);
}

void read_header(std::ifstream& file, uint32_t& channelmask,
		uint32_t& event_size)
{
	uint32_t word = 0;
	file.read((char*) &word, sizeof(uint32_t));
	event_size = word & 0x0fffffff;

	file.read((char*) &word, sizeof(uint32_t));
	channelmask = word & 0xFF;
	//dump next 2 words
	file.read((char*) &word, sizeof(uint32_t));
	file.read((char*) &word, sizeof(uint32_t));
}

size_t convert_single_event(std::ifstream& in_file, std::ofstream& out_file,
		const uint32_t word_count_per_channel, const uint16_t n_channels)
{
	size_t bytes_written = 0;
	//assert files are correct
	if (!in_file.is_open())
	{
		throw std::ios_base::failure("in_file must be open");
	}
	if (!out_file.is_open())
	{
		throw std::ios_base::failure("out_file must be open");
	}
	if (out_file.tellp() == std::ios::beg)
	{
		throw std::ios_base::failure("Write drift tube header to file first");
	}

	//write event
	uint32_t** event_words;
	try
	{
		event_words = new uint32_t*[n_channels];
		for (uint32_t i = 0; i < n_channels; ++i)
		{
			event_words[i] = new uint32_t[word_count_per_channel];
		}

		for (uint32_t i = 0; i < n_channels; ++i)
		{
			for (uint32_t j = 0; j < word_count_per_channel; ++j)
			{
				uint32_t word;
				uint16_t word_first_part, word_second_part;
				in_file.read((char*) &word, sizeof(uint32_t));

				event_words[i][j] = word;
			}
		}

		//write
		for (uint32_t channel = 0; channel < n_channels; ++channel)
		{
			for (uint32_t i = 0; i < word_count_per_channel; ++i)
			{
				out_file.write((char*) &event_words[channel][i],
						sizeof(uint32_t));
				bytes_written += sizeof(uint32_t);
			}
		}

		for (uint32_t i = 0; i < n_channels; ++i)
		{
			delete[] event_words[i];
		}
		delete[] event_words;
	} catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		if (event_words)
		{
			for (uint32_t i = 0; i < n_channels; ++i)
			{
				if (event_words[i])
					delete[] event_words[i];
			}
			delete[] event_words;
		}
	}

	return bytes_written;
}

std::streampos write_drifttube_header(std::ofstream& out_file,
		const uint32_t n_channels, const uint32_t n_events,
		const uint32_t event_size)
{
	//make sure header is at file beginning
	if (out_file.tellp() != std::ios::beg)
	{
		throw std::ios_base::failure(
				"Tried to write header at position different from file beginning");
	}

	if (out_file.is_open())
	{
		out_file.write((char*) &n_channels, sizeof(uint32_t));
		out_file.write((char*) &n_events, sizeof(uint32_t));
		out_file.write((char*) &event_size, sizeof(uint32_t));
	}

	return out_file.tellp();
}

bool next_event_complete(std::ifstream& in_file, const uint32_t n_channels,
		const uint32_t bytes_per_channel)
{
	std::streampos before_testing = in_file.tellg();
	for (uint32_t i = 0; i < n_channels * bytes_per_channel; ++i)
	{
		if (in_file.eof())
		{
			return false;
		}
		uint8_t byte = 0;
		in_file.read((char*)&byte, sizeof(byte));
	}
	in_file.seekg(before_testing);

	return true;
}
