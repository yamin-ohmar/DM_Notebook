const npcContainer = document.getElementById("npc-container");

function displayNPCs(npcs) {
    // Clear existing NPC cards
    npcContainer.innerHTML = "";

    // Loop through each NPC
    npcs.forEach((npc) => {
        // Create a new div element
        const card = document.createElement("div");

        // Give the div a class of "npc-card"
        card.className = "npc-card";// Create NPC name heading
        const name = document.createElement("h3");
        name.textContent = npc.name;

        // Create NPC race and role information
        const info = document.createElement("div");
        info.className = "npc-info";
        info.textContent = `${npc.race} - ${npc.role}`;

        // Create NPC description
        const description = document.createElement("p");
        description.textContent = npc.description;

        // Add the elements to the card
        card.append(name, info, description);

        // Append the card to npcContainer
        npcContainer.appendChild(card);
    });
}

// function addNPC() {
//     // TODO 1: Ask user for NPC details
//     const draftName = prompt("Enter NPC name:");
//     if (!draftName) return;
//     const name = draftName.trim();
//     if (!name) return;

//     const race = prompt("Enter NPC race (e.g. Elf, Draft, Human):");
//     const role = prompt("Enter NPC role (e.g. Fighter, Mage, Rogue):");
//     const description = prompt("Enter a brief description of the NPC:");

//     // TODO 2: Create an NPC object
//     const newNPC = {
//         id: Math.max(0, ...npcs.map(npc => npc.id)) + 1,
//         name: name,
//         race: race || "unknown",
//         role: role || "unknown",
//         description: description || "None"
//     };

//     // TODO 3: Add the object to the npcs array
//     npcs.push(newNPC);

//     // TODO 4: Refresh the displayed NPCs
//     displayNPCs();
// }

// async declares a function that returns a Promise and permits await inside it.
async function loadNPCs() {
    try {
        // Fetch NPCs from "/api/npcs"
        const response = await fetch("/api/npcs");      // Sends an HTTP GET request to Crow (Waits for an asynchronous operation)

        // Throw an Error if the request was unsuccessful
        if (!response.ok) {
            throw new Error("Failed to fetch NPCs");
        }

        // Convert the response body to JSON and store in a variable
        const npcs = await response.json();

        // Pass the array to display
        displayNPCs(npcs);

    } catch (error) {
        console.error("Failed to load NPCs:", error);
    }
}


// Call the function to load and display NPCs
loadNPCs();

// Event listener for 'Add NPC' button
// const addNpcBtn = document.getElementById("add-npc-btn");
// addNpcBtn.addEventListener("click", addNPC);
