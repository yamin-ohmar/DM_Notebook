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


async function addNPC() {
    // Ask for NPC details and validate the name
    const draftName = prompt("Enter NPC name:");
    if (!draftName) return;

    const name = draftName.trim();
    if (!name) return;

    const race = prompt("Enter NPC race (e.g. Elf, Dwarf, Human):") || "Unknown";
    const role = prompt("Enter NPC role (e.g. Fighter, Mage, Rogue):") || "Unknown";
    const description = prompt("Enter a brief NPC description:") || "None";

    // Create an object containing the NPC information
    const newNPC = {
        name: name,
        race: race,
        role: role,
        description: description
    };

    try {
        // Send a POST request to /api/npcs with a URL and an options object
        const response = await fetch("/api/npcs", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify(newNPC)    // converts JavaScript object into JSON text
        });

        // Throw an Error if the request was unsuccessful
        if (!response.ok) {
            throw new Error("Failed to create NPCs");
        }

        // Reload NPCs after successfully saving the new record
        await loadNPCs();

    } catch (error) {
        console.error("Failed to add NPC:", error);
        alert("Unable to add NPC. Please try again.");
    }
}

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


// Load NPCs when the webpage opens
loadNPCs();

// Handle "Add NPC" button clicks
const addNpcBtn = document.getElementById("add-npc-btn");
addNpcBtn.addEventListener("click", addNPC);
