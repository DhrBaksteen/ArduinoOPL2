const fs = require('fs');

const fileName = process.argv[2];
const fileData = Array.from(new Uint8Array(fs.readFileSync(fileName)));

const numUsed        = fileData[8] + fileData[9] * 256;
const numInstruments = fileData[10] + fileData[11] * 256;
const offsetNames    = fileData[12] + (fileData[13] << 8) + (fileData[14] << 16) + (fileData[15] << 24);
const offsetData     = fileData[16] + (fileData[17] << 8) + (fileData[18] << 16) + (fileData[19] << 24);
const instruments    = [];

for (let i = 0; i < numInstruments; i ++) {
	const offset = offsetNames + i * 12;
	instruments[i] = {};
	instruments[i].dataIndex = fileData[offset] + (fileData[offset + 1] << 8);
	instruments[i].name = "";
	for (let j = 0; j < 8; j ++) {
		instruments[i].name += String.fromCharCode(fileData[offset + 3 + j]);
	}
}


for (let i = 0; i < numInstruments; i ++) {
	const offset = offsetData + instruments[i].dataIndex * 30;
	instruments[i].percussionChannel = fileData[offset + 1];

	instruments[i].modProps = fileData[offset + 3] |
		(fileData[offset + 7] << 5) |
		(fileData[offset + 11] << 7) |
		(fileData[offset + 12] << 6) |
		(fileData[offset + 13] << 4);
	instruments[i].modLevel = (fileData[offset + 2] << 6) |
		fileData[offset + 10];
	instruments[i].modAttackDecay = (fileData[offset + 5] << 4) |
		fileData[offset + 8];
	instruments[i].modSustainRelease = (fileData[offset + 6] << 4) |
		fileData[offset + 9];
	instruments[i].modFeedback = (fileData[offset + 4] << 1) |
		(fileData[offset + 14] === 0 ? 0x01 : 0x00);
	instruments[i].modWaveSelect = fileData[offset + 28];

	instruments[i].carProps = fileData[offset + 16] |
		(fileData[offset + 20] << 5) |
		(fileData[offset + 24] << 7) |
		(fileData[offset + 25] << 6) |
		(fileData[offset + 26] << 4);
	instruments[i].carLevel = (fileData[offset + 15] << 6) |
		fileData[offset + 23];
	instruments[i].carAttackDecay = (fileData[offset + 18] << 4) |
		fileData[offset + 21];
	instruments[i].carSustainRelease = (fileData[offset + 19] << 4) |
		fileData[offset + 22];
	instruments[i].carWaveSelect = fileData[offset + 29];
}


for (let i = 0; i < numInstruments; i ++) {
	instrumentData = `const unsigned char ${ instruments[i].name.toUpperCase().replace(/[^A-Z0-9]/g, '_') }[12]  PROGMEM = { `;
	instrumentData += `${ toHex(instruments[i].percussionChannel) }, `;
	instrumentData += `${ toHex(instruments[i].modProps) }, `;
	instrumentData += `${ toHex(instruments[i].modLevel) }, `;
	instrumentData += `${ toHex(instruments[i].modAttackDecay) }, `;
	instrumentData += `${ toHex(instruments[i].modSustainRelease) }, `;
	instrumentData += `${ toHex(instruments[i].modFeedback) }, `;
	instrumentData += `${ toHex(instruments[i].modWaveSelect) }, `;

	instrumentData += `${ toHex(instruments[i].carProps) }, `;
	instrumentData += `${ toHex(instruments[i].carLevel) }, `;
	instrumentData += `${ toHex(instruments[i].carAttackDecay) }, `;
	instrumentData += `${ toHex(instruments[i].carSustainRelease) }, `;
	instrumentData += `${ toHex(instruments[i].carWaveSelect) } };`;
	console.log(instrumentData);
}

function toHex(dec) {
	hex = dec.toString(16).toUpperCase();
	return `0x${ hex.length < 2 ? '0' : '' }${hex}`;
}
